/*
 *  Template of an ordinary module
 */

#ifndef SONIFYSPATIAL_H
#define SONIFYSPATIAL_H

#include <Inventor/nodes/SoCube.h>
#include <hxcore/HxModule.h>
#include <hxcluster/HxCluster.h>
#include <hxcluster/HxClusterView.h>

#include <boost/asio.hpp>

#include "tinyosc++.h"
#include "api.h"


class SONIFYSPATIAL_API SonifySpatial : public HxClusterView
{
    HX_HEADER(SonifySpatial);

  public:
    SonifySpatial();
    ~SonifySpatial();
    static void mouseMoveEventCB(void *p, SoEventCallback *eventCB);
    void onMouseMoveEvent(SoEventCallback *eventCB);
    //static void mouseClickEventCB(void *p, SoEventCallback *eventCB);
    //void onMouseClickEvent(SoEventCallback *eventCB);
    virtual void compute();

  private:
    //HxPortFloatSlider portSize;
    HxPortText         portIP;       ///IP Address Field

    /// Used in fetching dataset elements
    HxCluster*         dataSet;
    bool               dataInit;
    char*              oscServerIp;
    bool               lastCamParams;
    float              lastCamPosX;
    float              lastCamPosY;
    float              lastCamPosZ;
    float              lastCamAxisX;
    float              lastCamAxisY;
    float              lastCamAxisZ;
    float              lastCamAngle;

    /// Private functions used internally
    void init();
    //void sendOSCPacket();
    void send(tnyosc::Message& msg);
    //void printEventSource(const SoLocation2Event* event);
    //void mouseOntoData();
    //void mouseOffData();
    void sendCameraParams();
};

#endif // SONIFYSPATIAL_H
