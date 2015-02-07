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

#include "tbb/parallel_do.h"
#include <vector>
#include <algorithm>
#include "Graph.h"


class Body {
public:
    Body() {};

    //------------------------------------------------------------------------
    // Following signatures are required by parallel_do
    //------------------------------------------------------------------------
    typedef Cell* argument_type;

    void operator()( Cell* c, tbb::parallel_do_feeder<Cell*>& feeder ) const {
        c->update();
        // Restore ref_count in preparation for subsequent traversal.
        c->ref_count = ArityOfOp[c->op];
        for( size_t k=0; k<c->successor.size(); ++k ) {
            Cell* successor = c->successor[k];
            // ref_count is used for inter-task synchronization.
            // Correctness checking tools might not take this into account, and report
            // data races between different tasks, that are actually synchronized.
            if( 0 == --(successor->ref_count) ) {
                feeder.add( successor );
            }
        }
    }
};

void ParallelPreorderTraversal( const std::vector<Cell*>& root_set ) {
    tbb::parallel_do(root_set.begin(), root_set.end(),Body());
}


