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

#ifndef __TBBexample_graph_logicsim_oba_H
#define __TBBexample_graph_logicsim_oba_H 1

#include "basics.h"

class one_bit_adder {
    broadcast_node<signal_t> A_port;
    broadcast_node<signal_t> B_port;
    broadcast_node<signal_t> CI_port;
    xor_gate<2> FirstXOR;
    xor_gate<2> SecondXOR;
    and_gate<2> FirstAND;
    and_gate<2> SecondAND;
    or_gate<2> FirstOR;
    graph& my_graph;
public:
    one_bit_adder(graph& g) : my_graph(g), A_port(g), B_port(g), CI_port(g), FirstXOR(g), 
                              SecondXOR(g), FirstAND(g), SecondAND(g), FirstOR(g) {
        make_connections();
    }
    one_bit_adder(const one_bit_adder& src) : 
        my_graph(src.my_graph), A_port(src.my_graph), B_port(src.my_graph), 
        CI_port(src.my_graph), FirstXOR(src.my_graph), SecondXOR(src.my_graph), 
        FirstAND(src.my_graph), SecondAND(src.my_graph), FirstOR(src.my_graph) 
    {
        make_connections();
    }
        
    ~one_bit_adder() {}
    receiver<signal_t>& get_A() { return A_port; }
    receiver<signal_t>& get_B() { return B_port; }
    receiver<signal_t>& get_CI() { return CI_port; }
    sender<signal_t>& get_out() {
        return SecondXOR.get_out();
    }
    sender<signal_t>& get_CO() {
        return FirstOR.get_out();
    }
private:
    void make_connections() {
        make_edge(A_port, FirstXOR.get_in(0));
        make_edge(A_port, FirstAND.get_in(0));
        make_edge(B_port, FirstXOR.get_in(1));
        make_edge(B_port, FirstAND.get_in(1));
        make_edge(CI_port, SecondXOR.get_in(1));
        make_edge(CI_port, SecondAND.get_in(1));
        make_edge(FirstXOR.get_out(), SecondXOR.get_in(0));
        make_edge(FirstXOR.get_out(), SecondAND.get_in(0));
        make_edge(SecondAND.get_out(), FirstOR.get_in(0));
        make_edge(FirstAND.get_out(), FirstOR.get_in(1));
    }
};

#endif /* __TBBexample_graph_logicsim_oba_H */
