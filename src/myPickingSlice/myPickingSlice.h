/*
 *  Template of an ordinary module
 */

#ifndef MYPICKINGSLICE_H
#define MYPICKINGSLICE_H

#include <Inventor/nodes/SoCube.h>

#include <hxcore/HxModule.h>
#include <hxcore/HxPortSeparator.h>
#include <hxcore/HxPortFloatSlider.h>
#include <hxcore/HxPortButtonList.h>
#include <hxcore/HxPortText.h>
#include <hxcore/HxPortDoIt.h>
#include "tinyosc++.h"
#include <boost/asio.hpp>
#include "api.h"
#include <hxcluster/HxClusterView.h>
#include <hxcore/HxPortRangeSlider.h>

class MYPICKINGSLICE_API myPickingSlice : public HxClusterView
{
    HX_HEADER(myPickingSlice);

  public:

    myPickingSlice();
    ~myPickingSlice();

    HxPortSeparator portLine;               ///separator line
    HxPortButtonList portAction1;           ///on/off button
    HxPortDoIt portAction2;                 ///apply button (remove?)
    HxPortMultiMenu portAction3;            ///data layer selection
    HxPortRangeSlider portAction4;          ///data constraint
    HxPortMultiMenu portAction5;            ///synth type
    HxPortText portIP;                      ///IP Address Field
    HxCluster* _my_cluster;
    int _my_picked_id;
    bool _on;
    int _data_layer;
    int numLayers;
    float layer_constraints[10][2]; ///find a better way to initialize or resize in constructor
    char* IP;

    void init();

    //virtual void compute();

    void sendOSC();

    void mymouseClick(SoEventCallback *eventCB);

    static void mymouseClickCB(void *p, SoEventCallback *eventCB);

    virtual void compute();

};

#endif // MYPICKINGSLICE_H
