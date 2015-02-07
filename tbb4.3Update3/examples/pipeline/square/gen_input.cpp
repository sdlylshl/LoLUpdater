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

#include <stdlib.h>
#include <stdio.h>
#include <stdexcept>

#if _WIN32
#include <io.h>
#ifndef F_OK
#define F_OK 0
#endif
#define access _access
#else
#include <unistd.h>
#endif

const long INPUT_SIZE = 1000000;

//! Generates sample input for square.cpp
void gen_input( const char *fname ) {
    long num = INPUT_SIZE;
    FILE *fptr = fopen(fname, "w");
    if(!fptr) {
        throw std::runtime_error("Could not open file for generating input");
    }

    int a=0;
    int b=1;
    for( long j=0; j<num; ++j ) {
        fprintf(fptr, "%u\n",a);
        b+=a;
        a=(b-a)%10000;
        if (a<0) a=-a;
    }

    if(fptr) {
        fclose(fptr);
    }
}

void generate_if_needed( const char *fname ) {
    if ( access(fname, F_OK) != 0 )
        gen_input(fname);
}
