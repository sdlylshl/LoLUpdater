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

/*
    The original source for this example is
    Copyright (c) 1994-2008 John E. Stone
    All rights reserved.

    Redistribution and use in source and binary forms, with or without
    modification, are permitted provided that the following conditions
    are met:
    1. Redistributions of source code must retain the above copyright
       notice, this list of conditions and the following disclaimer.
    2. Redistributions in binary form must reproduce the above copyright
       notice, this list of conditions and the following disclaimer in the
       documentation and/or other materials provided with the distribution.
    3. The name of the author may not be used to endorse or promote products
       derived from this software without specific prior written permission.

    THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS
    OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
    WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
    ARE DISCLAIMED.  IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY
    DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
    DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
    OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
    HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
    LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
    OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
    SUCH DAMAGE.
*/

#ifdef EMULATE_PTHREADS

#include <assert.h>
#include "pthread_w.h"

/*
    Basics
*/

int
pthread_create (pthread_t *thread, pthread_attr_t *attr, void *(*start_routine) (void *), void *arg)
{
    pthread_t th;

    if (thread == NULL) return EINVAL;
    *thread = NULL;

    if (start_routine == NULL) return EINVAL;

    th = (pthread_t) malloc (sizeof (pthread_s));
    memset (th, 0, sizeof (pthread_s));

    th->winthread_handle = CreateThread (
        NULL,
        0,
        (LPTHREAD_START_ROUTINE) start_routine,
        arg,
        0,
        &th->winthread_id);
    if (th->winthread_handle == NULL) return EAGAIN;  /*  GetLastError()  */

    *thread = th;
    return 0;
}

int
pthread_join (pthread_t th, void **thread_return)
{
    BOOL b_ret;
    DWORD dw_ret;

    if (thread_return) *thread_return = NULL;

    if ((th == NULL) || (th->winthread_handle == NULL)) return EINVAL;

    dw_ret = WaitForSingleObject (th->winthread_handle, INFINITE);
    if (dw_ret != WAIT_OBJECT_0) return ERROR_PTHREAD;  /*  dw_ret == WAIT_FAILED; GetLastError()  */

    if (thread_return) {
        BOOL e_ret;
        DWORD exit_val;
        e_ret = GetExitCodeThread (th->winthread_handle, &exit_val);
        if (!e_ret) return ERROR_PTHREAD;  /*  GetLastError()  */
        *thread_return = (void *)(size_t) exit_val;
    }

    b_ret = CloseHandle (th->winthread_handle);
    if (!b_ret) return ERROR_PTHREAD;  /*  GetLastError()  */
    memset (th, 0, sizeof (pthread_s));
    free (th);
    th = NULL;

    return 0;
}

void
pthread_exit (void *retval)
{
    /*  specific to PTHREAD_TO_WINTHREAD  */

    ExitThread ((DWORD) ((size_t) retval));  /*  thread becomes signalled so its death can be waited upon  */
    /*NOTREACHED*/
    assert (0); return;  /*  void fnc; can't return an error code  */
}

/*
    Mutex
*/

int
pthread_mutex_init (pthread_mutex_t *mutex, pthread_mutexattr_t *mutex_attr)
{
    InitializeCriticalSection (&mutex->critsec);
    return 0;
}

int
pthread_mutex_destroy (pthread_mutex_t *mutex)
{
    return 0;
}

int
pthread_mutex_lock (pthread_mutex_t *mutex)
{
    EnterCriticalSection (&mutex->critsec);
    return 0;
}

int
pthread_mutex_unlock (pthread_mutex_t *mutex)
{
    LeaveCriticalSection (&mutex->critsec);
    return 0;
}

#endif  /*  EMULATE_PTHREADS  */
