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

#include "video.h"
#include <cassert>
#include <stdio.h>
#include <iostream>
#include <pthread.h>

unsigned int* g_pImg = 0;
int g_sizex=0, g_sizey=0;
static video *g_video = 0;
static int g_fps = 0;
char *window_title=NULL;
#define WINDOW_TITLE_SIZE 256
int cocoa_update=0;

#include <sched.h>
#include <sys/time.h>
struct timeval g_time;

video::video()
    : red_mask(0xff0000), red_shift(16), green_mask(0xff00),
      green_shift(8), blue_mask(0xff), blue_shift(0), depth(24)
{
    assert(g_video == 0);
    g_video = this; title = "Video"; cocoa_update=1; updating = true; calc_fps = false;
}

bool video::init_window(int x, int y)
{
    g_sizex = x; g_sizey = y;
    g_pImg = new unsigned int[x*y];
    if( window_title==NULL )
        window_title = (char*)malloc(WINDOW_TITLE_SIZE);
    strncpy( window_title, title, WINDOW_TITLE_SIZE-1 );
    running = true;
    return true;
}

bool video::init_console()
{
    running = true;
    return true;
}

void video::terminate()
{
    if(calc_fps) {
        double fps = g_fps;
		struct timezone tz; struct timeval end_time; gettimeofday(&end_time, &tz);
		fps /= (end_time.tv_sec+1.0*end_time.tv_usec/1000000.0) - (g_time.tv_sec+1.0*g_time.tv_usec/1000000.0);
        printf("%s: %.1f fps\n", title, fps);
  	}
    g_video = 0; running = false;
    if(g_pImg) { delete[] g_pImg; g_pImg = 0; }
}

video::~video()
{
    if(g_video) terminate();
}

//! Count and display FPS count in titlebar
bool video::next_frame()
{
    if(calc_fps){
	    if(!g_fps) {
		    struct timezone tz; gettimeofday(&g_time, &tz);
	    }
        g_fps++;
    }
    struct timezone tz; struct timeval now_time; gettimeofday(&now_time, &tz);
    double sec=((now_time.tv_sec+1.0*now_time.tv_usec/1000000.0) - (g_time.tv_sec+1.0*g_time.tv_usec/1000000.0));
    if( sec>1 ){
        if(calc_fps) {
            memcpy(&g_time, &now_time, sizeof(g_time));
            int fps; 
            fps = g_fps/sec;
            cocoa_update = (int)updating;
            snprintf(window_title,WINDOW_TITLE_SIZE, "%s%s: %d fps", title, updating?"":" (no updating)", int(fps));
            g_fps=0;
        }
    }
    return running;
}


void* thread_func(void*)
{
    g_video->on_process();
    exit(EXIT_SUCCESS);
}

extern "C" void on_mouse_func(int x, int y, int k)
{
    g_video->on_mouse(x, y, k);
    return;
}
 
extern "C" void on_key_func(int x)
{
    g_video->on_key(x);
    return;
}

extern "C" int cocoa_main( int argc, char *argv[] );
//! Do standard loop
void video::main_loop()
{
    pthread_t handle;
    pthread_attr_t attr;
    pthread_attr_init(&attr);
    pthread_create(&handle,&attr,&thread_func,(void*)NULL);
    pthread_detach(handle);
    cocoa_main( 0, NULL );
}

//! Change window title
void video::show_title()
{
    strncpy( window_title, title, WINDOW_TITLE_SIZE );
    return;
}

///////////////////////////////////////////// public methods of video class ///////////////////////

drawing_area::drawing_area(int x, int y, int sizex, int sizey)
    : start_x(x), start_y(y), size_x(sizex), size_y(sizey), pixel_depth(24),
    base_index(y*g_sizex + x), max_index(g_sizex*g_sizey), index_stride(g_sizex), ptr32(g_pImg)
{
    assert(x < g_sizex); assert(y < g_sizey);
    assert(x+sizex <= g_sizex); assert(y+sizey <= g_sizey);

    index = base_index; // current index
}

void drawing_area::update() 
{
    //nothing to do, updating via timer in cocoa part. 
}
