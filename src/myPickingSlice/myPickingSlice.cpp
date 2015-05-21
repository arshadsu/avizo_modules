/*
 *  SoundProbe for HxCluster Data (Point Cloud)
 */
#include <iostream>
using std::cout;
using std::endl;
#include <QApplication>
#include <hxcore/HxMessage.h>
#include <hxfield/HxUniformScalarField3.h>
//#include <hxlattice/hxOrthoslice.h>
#include <hxcluster/HxCluster.h>
#include <hxcore/HxObjectPool.h>
#include "myPickingSlice.h"
#include "tinyosc++.h"
#include <boost/asio.hpp>
#include <Inventor/events/SoMouseButtonEvent.h>
#include <hxcluster/HxClusterView.h>
#include <hxvertexset/SoSphereDetail.h>
#include <Inventor/SoPickedPoint.h>
#include <Inventor/events/SoLocation2Event.h>

//#define HOST ("192.168.7.200")
#define HOST ("109.171.139.70")
#define PORT ("6900")

using boost::asio::ip::udp;

HX_INIT_CLASS(myPickingSlice,HxClusterView)

myPickingSlice::myPickingSlice() :
    HxClusterView(),
    portLine(this,"line1",QApplication::translate("soundProbe", "Line")),
    portAction1(this,"action1",QApplication::translate("soundProbe", "Sound Settings"),2),
    portVolume(this,"action6",QApplication::translate("soundProbe", "Volume")),
    portAction2(this,"action2",QApplication::translate("soundProbe", "Action2")),
    portAction3(this,"action3",QApplication::translate("soundProbe", "Data Layer"),6),
    portAction4(this,"action4",QApplication::translate("soundProbe", "Constrain Layer")),
    portAction5(this,"action5",QApplication::translate("soundProbe", "Synth Selection"),4),
    portIP(this,"action6",QApplication::translate("soundProbe", "Server IP Address"))

{
    soEventCB->addEventCallback(SoLocation2Event::getClassTypeId(),
        mymouseClickCB, this);
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
    portIP.setValue("109.171.139.70");


    //portAction4.setTextWidth(100);


    _my_picked_id = 1;
    _data_layer = -1;




///CAN'T PULL FROM _MY_CLUSTER HERE, ELSE SEGFAULT

}



myPickingSlice::~myPickingSlice()
{
}

void myPickingSlice::init()
{
    HxCluster* _my_cluster = (HxCluster*) portData.source();
    numLayers = _my_cluster->dataColumns.size();
    portAction3.setNum(numLayers);
    int i;
    for (i=0; i<numLayers; i++){
        portAction3.setLabel(i, QString(_my_cluster->dataColumns[i].name)); ///pull labels from data and set to GUI
        layer_constraints[i][0] = _my_cluster->dataColumns[i].min; ///populate array with initial values from data
        layer_constraints[i][1] = _my_cluster->dataColumns[i].max; ///populate array with initial values from data
        //printf("Data Layer Array %i 0: %f\n", i, layer_constraints[i][0]);
        //printf("Data Layer Array %i 1: %f\n", i, layer_constraints[i][1]);
    }


}

void myPickingSlice::mymouseClickCB(void *p, SoEventCallback *eventCB)
{
    ((myPickingSlice*)p)->mymouseClick(eventCB);
}



void myPickingSlice::mymouseClick(SoEventCallback *eventCB)
{
    ///Need to default idx to 0 when not over an actual point
    const SoLocation2Event *event =
        (SoLocation2Event*)eventCB->getEvent();

    const SoPickedPoint* pickedPoint = eventCB->getPickedPoint();
    if (pickedPoint == NULL)
        printf("AAAAAAH");
    if (!pickedPoint) {
        printf("nothing picked");
        std::cout<<"nope"<<endl;
        return;

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
    _my_picked_id = idx;
    SbVec2s pos = (event->getPosition());
    std::cout<<"mouse now at "<<pos[0]<<", "<<pos[1]<<endl;

     myPickingSlice::sendOSC();

}



void myPickingSlice::sendOSC()
{
    HxCluster* _my_cluster = (HxCluster*) portData.source();

     ///THIS BLOCK IS NOT COOL
    QString str1 = portIP.getValue();
    QByteArray ba = str1.toLatin1();
    IP = ba.data();
    ///END UNCOOL BLOCK

    ///THIS BLOCK PREVIOUSLY FROM SEND OSC FUNCTION

    boost::asio::io_service io_service;
    udp::socket socket(io_service, udp::endpoint(udp::v4(), 0));
    udp::resolver resolver(io_service);
    udp::resolver::query query(udp::v4(), IP, PORT); ///"IP" Arg used to be "HOST"
    udp::resolver::iterator iterator = resolver.resolve(query);
    /// create a OSC message
    tnyosc::Message msg("/data");
    msg.append(layer_constraints[_data_layer][0]);
    msg.append(layer_constraints[_data_layer][1]);
    msg.append(_my_cluster->dataColumns[_data_layer].getFloat(_my_picked_id));
    msg.append(portAction5.getValue(0));
    /// send the message
    socket.send_to(boost::asio::buffer(msg.data(), msg.size()), *iterator);

    /////////////////////////////////////////////////
    ///END BLOCK PREVIOUSLY FROM SEND OSC FUNCTION///
    /////////////////////////////////////////////////



}


void myPickingSlice::compute()
{
    HxCluster* _my_cluster = (HxCluster*) portData.source();
    static bool initialized = false;
    if (!initialized) {
    myPickingSlice::init();
    initialized = true;
    }

     ///THIS BLOCK IS NOT COOL
    QString str1 = portIP.getValue();
    QByteArray ba = str1.toLatin1();
    IP = ba.data();
    ///END UNCOOL BLOCK

    HxClusterView::compute();



    _data_layer = portAction3.getValue(0);
    portAction4.setClipValue(_my_cluster->dataColumns[_data_layer].min, _my_cluster->dataColumns[_data_layer].max); ///init sliders
    portAction4.setMinMax(_my_cluster->dataColumns[_data_layer].min, _my_cluster->dataColumns[_data_layer].max); ///init sliders

    if (portAction3.isNew()) {
        portAction4.setValue(layer_constraints[_data_layer][0],layer_constraints[_data_layer][1]);
        //printf("Data Layer: %i \nData Layer 0: %f \nData Layer 1: %f \nMin: %f \nMax: %f \n",_data_layer, layer_constraints[_data_layer][0], layer_constraints[_data_layer][1], _my_cluster->dataColumns[_data_layer].min, _my_cluster->dataColumns[_data_layer].max );
        //return;
    } else {
        layer_constraints[_data_layer][0] = portAction4.getValue(0);
        layer_constraints[_data_layer][1] = portAction4.getValue(1);
    }

    if (portVolume.isNew()) {
        theMsg->printf("Volume Updated\n");
        boost::asio::io_service io_service;
        udp::socket socket(io_service, udp::endpoint(udp::v4(), 0));
        udp::resolver resolver(io_service);
        udp::resolver::query query(udp::v4(), IP, PORT);///"IP" Arg used to be "HOST"
        udp::resolver::iterator iterator = resolver.resolve(query);
        /// create a OSC message
        tnyosc::Message msg("/volume");
        msg.append(portVolume.getValue());
        socket.send_to(boost::asio::buffer(msg.data(), msg.size()), *iterator);
    }
    if (portAction1.wasHit(1)) {
        theMsg->printf("SoundProbe Audio OFF\n");
        boost::asio::io_service io_service;
        udp::socket socket(io_service, udp::endpoint(udp::v4(), 0));
        udp::resolver resolver(io_service);
        udp::resolver::query query(udp::v4(), IP, PORT);///"IP" Arg used to be "HOST"
        udp::resolver::iterator iterator = resolver.resolve(query);
        /// create a OSC message
        tnyosc::Message msg("/global");
        msg.append(0);
        socket.send_to(boost::asio::buffer(msg.data(), msg.size()), *iterator);
    }
    if (portAction1.wasHit(0)) {
        theMsg->printf("SoundProbe Audio ON\n");
        boost::asio::io_service io_service;
        udp::socket socket(io_service, udp::endpoint(udp::v4(), 0));
        udp::resolver resolver(io_service);
        udp::resolver::query query(udp::v4(), IP, PORT);///"IP" Arg used to be "HOST"
        udp::resolver::iterator iterator = resolver.resolve(query);
        /// create a OSC message
        tnyosc::Message msg("/global");
        msg.append(1);
        socket.send_to(boost::asio::buffer(msg.data(), msg.size()), *iterator);
    }
    if (portAction2.wasHit()) {
        theMsg->printf("SoundProbe Loaded\n");
        boost::asio::io_service io_service;
        udp::socket socket(io_service, udp::endpoint(udp::v4(), 0));
        udp::resolver resolver(io_service);
        udp::resolver::query query(udp::v4(), IP, PORT);///"IP" Arg used to be "HOST"
        udp::resolver::iterator iterator = resolver.resolve(query);
        /// create a OSC message
        tnyosc::Message msg("/test");
        msg.append("hello from Avizo");
        msg.append(portAction1.getValue());
        socket.send_to(boost::asio::buffer(msg.data(), msg.size()), *iterator);
    }
    if (portAction5.isNew()) {
        theMsg->printf("SoundProbe Loaded\n");
        boost::asio::io_service io_service;
        udp::socket socket(io_service, udp::endpoint(udp::v4(), 0));
        udp::resolver resolver(io_service);
        udp::resolver::query query(udp::v4(), IP, PORT);///"IP" Arg used to be "HOST"
        udp::resolver::iterator iterator = resolver.resolve(query);
        /// create a OSC message
        tnyosc::Message msg("/selection");
        msg.append(portAction5.getValue(0));
        socket.send_to(boost::asio::buffer(msg.data(), msg.size()), *iterator);
    }








}


