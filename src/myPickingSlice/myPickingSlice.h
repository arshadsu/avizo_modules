/*
 *  Template of an ordinary module
 */

#ifndef MYPICKINGSLICE_H
#define MYPICKINGSLICE_H

#include <Inventor/nodes/SoCube.h>

#include <hxcore/HxModule.h>
#include <hxcore/HxPortFloatSlider.h>
#include <hxcore/HxPortDoIt.h>
#include "tinyosc++.h"
#include <boost/asio.hpp>
#include "api.h"
#include <hxcluster/HxClusterView.h>

class MYPICKINGSLICE_API myPickingSlice : public HxClusterView
{
    HX_HEADER(myPickingSlice);

  public:

    myPickingSlice();
    ~myPickingSlice();

    HxPortDoIt portAction;
    //HxCluster _my_cluster;
    int _my_picked_id;

    virtual void compute();

    void sendOSC();

    void mymouseClick(SoEventCallback *eventCB);

    static void mymouseClickCB(void *p, SoEventCallback *eventCB);

};

#endif // MYPICKINGSLICE_H
