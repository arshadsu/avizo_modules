/*
 *  Template of an ordinary module
 */

#include <iostream>

#include <QApplication>
#include <hxcore/HxMessage.h>
#include <hxcore/HxController.h>

#include "SonifySpatial.h"

//#define RIKERPW_OSC_SERVER  "109.171.139.70"
#define RIKERPW_OSC_SERVER  "109.171.139.97"
#define ARSHADSU_OSC_SERVER "109.171.139.88"
#define DEFAULT_OSC_SERVER  ARSHADSU_OSC_SERVER
//#define DEFAULT_OSC_SERVER  RIKERPW_OSC_SERVER
#define DEFAULT_OSC_PORT    "6900"
#define EPSILON             0.00001f
#define DEBUG               1

using namespace std;
using boost::asio::ip::udp;


HX_INIT_CLASS(SonifySpatial,HxClusterView)


//////////
// Constructor
//   Calls parent class constructor and then sets up the UI elements
//////////
SonifySpatial::SonifySpatial() :
    HxClusterView()
  , portIP(this,"action6",QApplication::translate("soundProbe", "Server IP Address"))
  , dataInit(false)
  , oscServerIp(NULL)
  , sensor(NULL)
{
    portIP.setValue(DEFAULT_OSC_SERVER);
    portIP.setState(DEFAULT_OSC_SERVER);
}


//////////
// Destructor
//   
//////////
SonifySpatial::~SonifySpatial()
{
    delete sensor;
}


//////////
//   This is actually mouse move callback
//////////
void SonifySpatial::mouseMoveEventCB(void *p, SoEventCallback *eventCB)
{
    ((SonifySpatial*)p)->onMouseMoveEvent(eventCB);
}


//////////
//   Handles mouse event
//////////
void SonifySpatial::onMouseMoveEvent(SoEventCallback *eventCB)
{
    sendCameraParams();
}


//////////
//   Called once when dataSet has been loaded to perform intial setup
//////////
void SonifySpatial::init()
{
    dataSet = (HxCluster*) portData.source();

    /// send the points to sound server once
    theMsg->printf("Sending points to sound server\n");

    {
        /// create a OSC message
        tnyosc::Message msg("/mode");
        msg.append(0);
        /// send the message
        send(msg);
    }

    int numPoints = dataSet->getNumPoints();

    {
        /// create a OSC message
        tnyosc::Message msg("/size");
        msg.append(numPoints);
        /// send the message
        send(msg);
    }

    for (int i = 0; i < numPoints; ++i)
    {
        /// create a OSC message
        tnyosc::Message msg("/coordinates");
        msg.append(i);
        msg.append(dataSet->points[i].x);
        msg.append(dataSet->points[i].y);
        msg.append(dataSet->points[i].z);
        /// send the message
        send(msg);
    }

    sendCameraParams();

    SoCamera* camera = theController->getCurrentViewer()->getCamera();
    sensor = new SoNodeSensor(cameraChangeEventCB, this);
    sensor->attach(camera);
}


void SonifySpatial::cameraChangeEventCB(void *p, SoSensor* s)
{
 ((SonifySpatial*)p)->sendCameraParams();
}

void SonifySpatial::sendCameraParams()
{
    HxViewer* viewer = theController->getCurrentViewer();
    SoCamera* camera = viewer->getCamera();
    float camPosX = 0.0f, camPosY = 0.0f, camPosZ = 0.0f;
    camera->position.getValue().getValue(camPosX, camPosY, camPosZ);
    SbVec3f camAxis;
    float camAxisX = 0.0f, camAxisY = 1.0f, camAxisZ = 0.0f;
    float camAngle;
    camera->orientation.getValue(camAxis, camAngle);
    camAxis.getValue(camAxisX, camAxisY, camAxisZ);
    {
        /// create a OSC message
        tnyosc::Message msg("/camerapos");
        msg.append(camPosX);
        msg.append(camPosY);
        msg.append(camPosZ);
        /// send the message
        send(msg);
    }
    {
        /// create a OSC message
        tnyosc::Message msg("/cameraorient");
        msg.append(camAxisX);
        msg.append(camAxisY);
        msg.append(camAxisZ);
        msg.append(camAngle);
        /// send the message
        send(msg);
    }
}


void SonifySpatial::compute()
{
#if DEBUG
    cout << "SonifySpatial::compute():" << endl;
#endif
    /// Ensure that data we point to loaded dataset, but do that once only
    if (!dataInit) {
        init();
        dataInit = true;
    }
    /// call parent class function
    HxClusterView::compute();
}


//////////
//   Sends the given OSC packet to the server, using Boost Asynchronous Library
//////////
void SonifySpatial::send(tnyosc::Message& msg)
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

