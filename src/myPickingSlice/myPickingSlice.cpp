/*
 *  SoundProbe for HxCluster Data (Point Cloud)
 */
#include <iostream>

#include <QApplication>
#include <hxcore/HxMessage.h>
#include <hxcore/HxObjectPool.h>
#include <hxcore/HxController.h>
#include <hxcluster/HxCluster.h>
#include <hxcluster/HxClusterView.h>
#include <hxfield/HxUniformScalarField3.h>
#include <hxvertexset/SoSphereDetail.h>
#include <Inventor/SoPickedPoint.h>
#include <Inventor/events/SoLocation2Event.h>
#include <Inventor/events/SoMouseButtonEvent.h>

#include "myPickingSlice.h"


//#define DEFAULT_OSC_SERVER  "109.171.139.70"
#define RIKERPW_OSC_SERVER  "109.171.139.70"
#define ARSHADSU_OSC_SERVER "109.171.139.88"
#define DEFAULT_OSC_SERVER  ARSHADSU_OSC_SERVER
#define DEFAULT_OSC_PORT    "6900"
#define DEBUG               0

using std::cout;
using std::endl;
using boost::asio::ip::udp;


HX_INIT_CLASS(myPickingSlice,HxClusterView)


//////////
// Constructor
//   Calls parent class constructor and then sets up the UI elements
//////////
myPickingSlice::myPickingSlice() :
    HxClusterView(),
    portLine(this,"line1",QApplication::translate("soundProbe", "Line")),
    portAction1(this,"action1",QApplication::translate("soundProbe", "Sound Settings"),2),
    portVolume(this,"action6",QApplication::translate("soundProbe", "Volume")),
    portAction2(this,"action2",QApplication::translate("soundProbe", "Action2")),
    portAction3(this,"action3",QApplication::translate("soundProbe", "Data Layer"),6),
    portAction4(this,"action4",QApplication::translate("soundProbe", "Constrain Layer")),
    portAction5(this,"action5",QApplication::translate("soundProbe", "Synth Selection"),4),
    portIP(this,"action6",QApplication::translate("soundProbe", "Server IP Address")),
    dataInit(false),
    pickedPrimIdx(1),
    numLayers(0),
    layerIdx(-1),
    oscServerIp(NULL),
    mouseOnData(false)
{
    theController->addMouseMoveCallback(mouseMoveEventCB, this);
    //soEventCB->addEventCallback(SoLocation2Event::getClassTypeId(),
    //                            mouseMoveEventCB, this);
    //theController->addPickCallback(mouseClickEventCB, (void*)this);
    portAction1.setLabel(0,"On");
    portAction1.setLabel(1,"Off");
    portVolume.setMinMax(0,1);
    portVolume.setValue(0.75);
    portAction2.setLabel(0,"DoIt");
    portAction3.setNum(2);
    portAction3.setLabel(0, "first layer");
    portAction3.setLabel(1, "second layer");
    portAction4.setMinMax(0, 1);
    portAction4.setEditButton(0);
    portAction4.enableMinMaxRestriction(true);
    portAction5.setLabel(0, "Density Synth");
    portAction5.setLabel(1, "Frequency Synth");
    portAction5.setLabel(2, "Intensity Synth");
    portAction5.setLabel(3, "Rhythmic Synth");
    portIP.setValue(DEFAULT_OSC_SERVER);
    //portAction4.setTextWidth(100);
}


//////////
// Destructor
//   No clean up needed for now
//////////
myPickingSlice::~myPickingSlice()
{
}


//////////
//   Called once when dataSet has been loaded to fetch the pointer, get the
//   layers/fields and their attributes
//////////
void myPickingSlice::init()
{
    dataSet = (HxCluster*) portData.source();
    numLayers = dataSet->dataColumns.size();
    portAction3.setNum(numLayers);
    for (int i = 0; i < numLayers; i++) {
        ///pull labels from data and set to GUI
        portAction3.setLabel(i, QString(dataSet->dataColumns[i].name));
        ///get min and max values for all data layers
        layerConstraints.push_back(std::pair<float,float>(dataSet->dataColumns[i].min,
                                                          dataSet->dataColumns[i].max));
    }
}

//////////
//   This is actually mouse move callback
//////////

void myPickingSlice::mouseMoveEventCB(void *p, SoEventCallback *eventCB)
{
    ((myPickingSlice*)p)->onMouseMoveEvent(eventCB);
}

void myPickingSlice::mouseClickEventCB(void *p, SoEventCallback *eventCB)
{
    ((myPickingSlice*)p)->onMouseClickEvent(eventCB);
}

void myPickingSlice::onMouseClickEvent(SoEventCallback *eventCB)
{
#if DEBUG
    cout << "Received click event" << endl;
#endif
    const SoLocation2Event *event = (SoLocation2Event*)eventCB->getEvent();
#if DEBUG
    SbVec2s pos = (event->getPosition());
    cout <<"** mouse now at "<< pos[0] << ", " << pos[1] << endl;
#endif
    const SoPickedPoint* pickedPoint = eventCB->getPickedPoint();
    if (!pickedPoint) {
#if DEBUG
        cout << "No picked point in mouse event" << endl;
#endif
        return;
    }
}


//////////
//   Handles mouse event
//////////
void myPickingSlice::onMouseMoveEvent(SoEventCallback *eventCB)
{
    ///Need to default idx to 0 when not over an actual point
    const SoLocation2Event *event =
        (SoLocation2Event*)eventCB->getEvent();
#if DEBUG
    SbVec2s pos = (event->getPosition());
    cout <<"** mouse now at "<< pos[0] << ", " << pos[1] << endl;
#endif
    const SoPickedPoint* pickedPoint = eventCB->getPickedPoint();
    if (!pickedPoint) {
#if DEBUG
        cout << "No picked point in mouse event" << endl;
#endif
        bool prevState = mouseOnData;
        mouseOnData = false;
        if (prevState) {
            // mouse pointer just moved off-data from on-data region
            mouseOffData();
        }
        return;
    }
    else {
        bool prevState = mouseOnData;
        mouseOnData = true;
        if (!prevState) {
            // mouse pointer just moved on-data from off-data region
            mouseOntoData();           
        }
    }

    const SoDetail *pickDetail = pickedPoint->getDetail();
    if (!pickDetail)
        return;

    int idx = ((SoSphereDetail *)pickDetail)->getSphereIndex();
    if (event->wasShiftDown()) {
        theMsg->printf("Deselected vertex %d" , idx);
        selection.unset(idx);
        soCluster->selection.setBit(idx, 0);
        soSphereSet->selected.setBit(idx, false);
    } else {
        theMsg->printf("Picked vertex %d" , idx);
    }
    pickedPrimIdx = idx;
    myPickingSlice::sendOSCPacket();

}


//////////
//    Do whatever needs to be done when mouse pointer moves onto data
//    from off-data region
//////////
void myPickingSlice::mouseOntoData()
{
}


//////////
//    Do whatever needs to be done when mouse pointer moves off data
//    from on-data region
//////////
void myPickingSlice::mouseOffData()
{
}


//////////
//    Helper function to print mouse event info on stdout
//////////
void myPickingSlice::printEventSource(const SoLocation2Event* event)
{
    if (event) {
        SoLocation2Event::EventSource src = event->getEventSource();
        switch(src)
        {
        case SoLocation2Event::MOUSE_MOVE:
            std::cout << "onMouseMoveEvent: MOUSE_MOVE" << std::endl;
            break;
        case SoLocation2Event::MOUSE_ENTER:
            std::cout << "onMouseMoveEvent: MOUSE_ENTER" << std::endl;
            break;
        case SoLocation2Event::MOUSE_LEAVE:
            std::cout << "onMouseMoveEvent: MOUSE_LEAVE" << std::endl;
            break;
        case SoLocation2Event::OTHER:
            std::cout << "onMouseMoveEvent: OTHER" << std::endl;
            break;
        default:
            std::cerr << "onMouseMoveEvent: Unknown EventSource" << std::endl;
            return;
        }
    }
}


//////////
//   Sends base OSC packet
//////////
void myPickingSlice::sendOSCPacket()
{
    /// create a OSC message
    tnyosc::Message msg("/data");
    msg.append(layerConstraints[layerIdx].first);
    msg.append(layerConstraints[layerIdx].second);
    msg.append(dataSet->dataColumns[layerIdx].getFloat(pickedPrimIdx));
    msg.append(portAction5.getValue(0));
    /// send the message
    send(msg);
}


//////////
//    Called whenever a new selection is made in the UI so that new data data
//    from selected layer/field is used for sonification
//////////
void myPickingSlice::compute()
{
#if DEBUG
    cout << "myPickingSlice::compute():" << endl;
#endif
    /// Ensure that data we point to loaded dataset, but do that once only
    if (!dataInit) {
        init();
        dataInit = true;
    }
    /// call parent class function
    HxClusterView::compute();
    /// get the data layer index
    layerIdx = portAction3.getValue(0);
    /// reset the slider min and max to current data layer
    portAction4.setClipValue(dataSet->dataColumns[layerIdx].min, 
                             dataSet->dataColumns[layerIdx].max);
    portAction4.setMinMax(dataSet->dataColumns[layerIdx].min, 
                          dataSet->dataColumns[layerIdx].max);
    /// for new data update the slider otherwise just refetch the min and max
    if (portAction3.isNew()) {
        portAction4.setValue(layerConstraints[layerIdx].first, 
                             layerConstraints[layerIdx].second);
    } else {
        layerConstraints[layerIdx].first  = portAction4.getValue(0);
        layerConstraints[layerIdx].second = portAction4.getValue(1);
    }

    if (portVolume.isNew()) {
        theMsg->printf("Volume Updated\n");
        /// create a OSC message
        tnyosc::Message msg("/volume");
        msg.append(portVolume.getValue());
        /// send the message
        send(msg);
    }
    if (portAction1.wasHit(1)) {
        theMsg->printf("SoundProbe Audio OFF\n");
        /// create a OSC message
        tnyosc::Message msg("/global");
        msg.append(0);
        /// send the message
        send(msg);
    }
    if (portAction1.wasHit(0)) {
        theMsg->printf("SoundProbe Audio ON\n");
        /// create a OSC message
        tnyosc::Message msg("/global");
        msg.append(1);
        /// send the message
        send(msg);
    }
    if (portAction2.wasHit()) {
        theMsg->printf("SoundProbe Loaded\n");
        /// create a OSC message
        tnyosc::Message msg("/test");
        msg.append("hello from Avizo");
        msg.append(portAction1.getValue());
        /// send the message
        send(msg);
    }
    if (portAction5.isNew()) {
        theMsg->printf("SoundProbe Loaded\n");
        /// create a OSC message
        tnyosc::Message msg("/selection");
        msg.append(portAction5.getValue(0));
        /// send the message
        send(msg);
    }
}


//////////
//   Sends the given OSC packet to the server, using Boost Asynchronous Library
//////////
void myPickingSlice::send(tnyosc::Message& msg)
{
    /// get the current OSC server IP address as char*
    QString str1 = portIP.getValue();
    QByteArray ba = str1.toLatin1();
    oscServerIp = ba.data();
    /// create the socket and UDP packet from the received message, and ship it to the OSC server
    boost::asio::io_service io_service;
    udp::socket socket(io_service, udp::endpoint(udp::v4(), 0));
    udp::resolver resolver(io_service);
    udp::resolver::query query(udp::v4(), oscServerIp, DEFAULT_OSC_PORT);///"IP" Arg used to be "HOST"
    udp::resolver::iterator iterator = resolver.resolve(query);
    socket.send_to(boost::asio::buffer(msg.data(), msg.size()), *iterator);
}

