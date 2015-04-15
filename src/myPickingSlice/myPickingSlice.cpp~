/*
 *  SoundProbe for HxCluster Data (Point Cloud)
 */

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

#define HOST ("109.171.139.70")
#define PORT ("6900")

using boost::asio::ip::udp;

HX_INIT_CLASS(myPickingSlice,HxClusterView)

myPickingSlice::myPickingSlice() :
    HxClusterView(),
    portAction(this,"action",QApplication::translate("soundProbe", "Action"))

{
    soEventCB->addEventCallback(SoLocation2Event::getClassTypeId(),
        mymouseClickCB, this);
    portAction.setLabel(0,"DoIt");
    _my_picked_id = 1;

}



myPickingSlice::~myPickingSlice()
{
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

    //if (!SO_MOUSE_PRESS_EVENT(event,BUTTON1))
    //    return;

    const SoPickedPoint* pickedPoint = eventCB->getPickedPoint();
    if (!pickedPoint)
        return;

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
    myPickingSlice::sendOSC();

}

void myPickingSlice::sendOSC()
{
        HxCluster* _my_cluster = (HxCluster*) portData.source();


        boost::asio::io_service io_service;
        udp::socket socket(io_service, udp::endpoint(udp::v4(), 0));
        udp::resolver resolver(io_service);
        udp::resolver::query query(udp::v4(), HOST, PORT);
        udp::resolver::iterator iterator = resolver.resolve(query);
        // create a OSC message
        tnyosc::Message msg("/data");
        //msg.append("hello tnyosc");
        //msg.append(_my_picked_id);
        //msg.append(_my_cluster->getNumPoints());
        msg.append(_my_cluster->dataColumns[6].getFloat(_my_picked_id)); ///********************
        // send the message
        socket.send_to(boost::asio::buffer(msg.data(), msg.size()), *iterator);
}

void myPickingSlice::compute()
{

    if (portAction.wasHit()) {
        theMsg->printf("SoundProbe Loaded\n");
        boost::asio::io_service io_service;
        udp::socket socket(io_service, udp::endpoint(udp::v4(), 0));
        udp::resolver resolver(io_service);
        udp::resolver::query query(udp::v4(), HOST, PORT);
        udp::resolver::iterator iterator = resolver.resolve(query);
        // create a OSC message
        tnyosc::Message msg("/test");
        msg.append("hello from Avizo");
        socket.send_to(boost::asio::buffer(msg.data(), msg.size()), *iterator);
    }
    HxClusterView::compute();
}


