/*
    Copyright 2005-2015 Intel Corporation.  All Rights Reserved.

    The source code contained or described herein and all documents related
    to the source code ("Material") are owned by Intel Corporation or its
    suppliers or licensors.  Title to the Material remains with Intel
    Corporation or its suppliers and licensors.  The Material is protected
    by worldwide copyright laws and treaty provisions.  No part of the
    Material may be used, copied, reproduced, modified, published, uploaded,
    posted, transmitted, distributed, or disclosed in any way without
    Intel's prior express written permission.

    No license under any patent, copyright, trade secret or other
    intellectual property right is granted to or conferred upon you by
    disclosure or delivery of the Materials, either expressly, by
    implication, inducement, estoppel or otherwise.  Any license under such
    intellectual property rights must be express and approved by Intel in
    writing.
*/

#ifndef SEISMIC_VIDEO_H_
#define SEISMIC_VIDEO_H_

#include "../../common/gui/video.h"

class Universe;

class SeismicVideo : public video
{
#ifdef _WINDOWS
    #define MAX_LOADSTRING 100
    TCHAR szWindowClass[MAX_LOADSTRING];    // the main window class name
    WNDCLASSEX wcex;
#endif
    static const char * const titles[2];

    bool initIsParallel ;

    Universe &u_;
    int numberOfFrames_; // 0 means forever, positive means number of frames, negative is undefined
    int threadsHigh;
private:
    void on_mouse(int x, int y, int key);
    void on_process();

#ifdef _WINDOWS
public:
#endif
    void on_key(int key);

public:
    SeismicVideo(    Universe &u,int numberOfFrames, int threadsHigh, bool initIsParallel=true);
};
#endif /* SEISMIC_VIDEO_H_ */
