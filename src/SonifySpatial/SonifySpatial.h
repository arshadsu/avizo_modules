/*
 *  Template of an ordinary module
 */

#ifndef SONIFYSPATIAL_H
#define SONIFYSPATIAL_H

#include <Inventor/nodes/SoCube.h>
#include <Inventor/sensors/SoNodeSensor.h>
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
    static void cameraChangeEventCB(void *p, SoSensor* s);
    virtual void compute();
    void sendCameraParams();

  private:
    //HxPortFloatSlider portSize;
    HxPortText         portIP;       ///IP Address Field

    /// Used in fetching dataset elements
    HxCluster*         dataSet;
    bool               dataInit;
    char*              oscServerIp;
    SoNodeSensor*      sensor;

    /// Private functions used internally
    void init();
    void send(tnyosc::Message& msg);
};

#endif // SONIFYSPATIAL_H
