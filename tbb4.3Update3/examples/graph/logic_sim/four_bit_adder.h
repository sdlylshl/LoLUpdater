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

#ifndef __TBBexample_graph_logicsim_fba_H
#define __TBBexample_graph_logicsim_fba_H 1

#include "one_bit_adder.h"

class four_bit_adder {
    graph& my_graph;
    std::vector<one_bit_adder> four_adders; 
 public:
    four_bit_adder(graph& g) : my_graph(g), four_adders(4, one_bit_adder(g)) {
        make_connections();
    }
    four_bit_adder(const four_bit_adder& src) : 
        my_graph(src.my_graph), four_adders(4, one_bit_adder(src.my_graph)) 
    {
        make_connections();
    }
    ~four_bit_adder() {}
    receiver<signal_t>& get_A(size_t bit) {
        return four_adders[bit].get_A();
    }
    receiver<signal_t>& get_B(size_t bit) {
        return four_adders[bit].get_B();
    }
    receiver<signal_t>& get_CI() {
        return four_adders[0].get_CI();
    }
    sender<signal_t>& get_out(size_t bit) {
        return four_adders[bit].get_out();
    }
    sender<signal_t>& get_CO() {
        return four_adders[3].get_CO();
    }
private:
    void make_connections() {
        make_edge(four_adders[0].get_CO(), four_adders[1].get_CI());
        make_edge(four_adders[1].get_CO(), four_adders[2].get_CI());
        make_edge(four_adders[2].get_CO(), four_adders[3].get_CI());
    }
};

#endif /* __TBBexample_graph_logicsim_fba_H */
