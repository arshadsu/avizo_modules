/*
 *  Template of an ordinary module
 */

#ifndef MYPICKINGSLICE_H
#define MYPICKINGSLICE_H

#include <Inventor/nodes/SoCube.h>

#include <hxcore/HxModule.h>
#include <hxcore/HxPortFloatSlider.h>
#include <hxcore/HxPortButtonList.h>
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

    HxPortButtonList portAction1;
    HxPortDoIt portAction2;
    HxPortMultiMenu portAction3;
    HxPortRangeSlider portAction4;
    //HxCluster _my_cluster;
    int _my_picked_id;
    bool _on;
    int _data_layer;


    virtual void compute();

    void sendOSC();

    void mymouseClick(SoEventCallback *eventCB);

    static void mymouseClickCB(void *p, SoEventCallback *eventCB);

};

#endif // MYPICKINGSLICE_H
