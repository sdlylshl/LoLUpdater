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

#ifndef UNIVERSE_H_
#define UNIVERSE_H_

#ifndef UNIVERSE_WIDTH
#define UNIVERSE_WIDTH 1024
#endif
#ifndef UNIVERSE_HEIGHT
#define UNIVERSE_HEIGHT 512
#endif

#include "../../common/gui/video.h"
#include "tbb/partitioner.h"

class Universe {
public:
    enum {
        UniverseWidth  = UNIVERSE_WIDTH,
        UniverseHeight = UNIVERSE_HEIGHT
    };
private:
    //in order to avoid performance degradation due to cache aliasing issue
    //some padding is needed after each row in array, and between array themselves.
    //the padding is achieved by adjusting number of rows and columns.
    //as the compiler is forced to place class members of the same clause in order of the
    //declaration this seems to be the right way of padding.

    //magic constants added below are chosen experimentally for 1024x512.
    enum {
        MaxWidth = UniverseWidth+1,
        MaxHeight = UniverseHeight+3
    };

    typedef float ValueType;

    //! Horizontal stress
    ValueType S[MaxHeight][MaxWidth];

    //! Velocity at each grid point
    ValueType V[MaxHeight][MaxWidth];

    //! Vertical stress
    ValueType T[MaxHeight][MaxWidth];

    //! Coefficient related to modulus
    ValueType M[MaxHeight][MaxWidth];

    //! Damping coefficients
    ValueType D[MaxHeight][MaxWidth];

    //! Coefficient related to lightness
    ValueType L[MaxHeight][MaxWidth];

    enum { ColorMapSize = 1024};
    color_t ColorMap[4][ColorMapSize];

    enum MaterialType {
        WATER=0,
        SANDSTONE=1,
        SHALE=2
    };

    //! Values are MaterialType, cast to an unsigned char to save space.
    unsigned char material[MaxHeight][MaxWidth];

private:
    enum { DamperSize = 32};

    int pulseTime;
    int pulseCounter;
    int pulseX;
    int pulseY;

    drawing_memory drawingMemory;

public:
    void InitializeUniverse(video const& colorizer);

    void SerialUpdateUniverse();
    void ParallelUpdateUniverse();
    bool TryPutNewPulseSource(int x, int y);
    void SetDrawingMemory(const drawing_memory &dmem);
private:
    struct Rectangle;
    void UpdatePulse();
    void UpdateStress(Rectangle const& r );

    void SerialUpdateStress() ;
    friend struct UpdateStressBody;
    friend struct UpdateVelocityBody;
    void ParallelUpdateStress(tbb::affinity_partitioner &affinity);

    void UpdateVelocity(Rectangle const& r);

    void SerialUpdateVelocity() ;
    void ParallelUpdateVelocity(tbb::affinity_partitioner &affinity);
};

#endif /* UNIVERSE_H_ */
