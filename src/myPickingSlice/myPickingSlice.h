/*
 *  Template of an ordinary module
 */

#ifndef MYPICKINGSLICE_H
#define MYPICKINGSLICE_H

#include <vector>

#include <boost/asio.hpp>
#include <hxcore/HxModule.h>
#include <hxcore/HxPortSeparator.h>
#include <hxcore/HxPortFloatSlider.h>
#include <hxcore/HxPortButtonList.h>
#include <hxcore/HxPortText.h>
#include <hxcore/HxPortDoIt.h>
#include <hxcluster/HxClusterView.h>
#include <hxcore/HxPortRangeSlider.h>

#include "tinyosc++.h"
#include "api.h"


typedef std::vector< std::pair<float,float> > PairVector;


class MYPICKINGSLICE_API myPickingSlice : public HxClusterView
{
    HX_HEADER(myPickingSlice);

  public:
    myPickingSlice();
    ~myPickingSlice();
    static void mouseEventCB(void *p, SoEventCallback *eventCB);
    void onMouseEvent(SoEventCallback *eventCB);
    static void mouseClickEventCB(void *p, SoEventCallback *eventCB);
    void onMouseClickEvent(SoEventCallback *eventCB);
    virtual void compute();

  private:
    /// UI elements
    HxPortSeparator    portLine;     ///separator line
    HxPortButtonList   portAction1;  ///on/off button
    HxPortFloatSlider  portVolume;   ///Volume Control
    HxPortDoIt         portAction2;  ///apply button (remove?)
    HxPortMultiMenu    portAction3;  ///data layer selection
    HxPortRangeSlider  portAction4;  ///data constraint
    HxPortMultiMenu    portAction5;  ///synth type
    HxPortText         portIP;       ///IP Address Field

    /// Used in fetching dataset elements
    HxCluster*         dataSet;
    bool               dataInit;
    int                pickedPrimIdx;
    int                numLayers;
    int                layerIdx;
    PairVector         layerConstraints;
    char*              oscServerIp;

    /// Private functions used internally
    void init();
    void sendOSCPacket();
    void send(tnyosc::Message& msg);
    void printEventSource(const SoLocation2Event* event);
};

#endif // MYPICKINGSLICE_H
