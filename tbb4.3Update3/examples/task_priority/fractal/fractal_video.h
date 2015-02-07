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

#ifndef FRACTAL_VIDEO_H_
#define FRACTAL_VIDEO_H_

#include "../../common/gui/video.h"
#include "fractal.h"

extern video *v;
extern bool single;

class fractal_video : public video
{
    fractal_group *fg;

private:
    void on_mouse( int x, int y, int key ) {
        if( key == 1 ) {
            if ( fg ) {
                fg->set_num_frames_at_least(20);
                fg->mouse_click( x, y );
            }
        }
    }

    void on_key( int key ) {
        switch ( key&0xff ) {
        case 27:
            running = false; break;
        case ' ': // space
            if( fg ) fg->switch_priorities();
        default:
            if( fg ) fg->set_num_frames_at_least(20);
        }
    }

    void on_process() {
        if ( fg ) {
            fg->run( !single );
        }
    }

public:
    fractal_video() :fg(0) {
        title = "Dynamic Priorities in TBB: Fractal Example";
        v = this;
    }

    void set_fractal_group( fractal_group &_fg ) {
        fg = &_fg;
    }
};

#endif /* FRACTAL_VIDEO_H_ */
