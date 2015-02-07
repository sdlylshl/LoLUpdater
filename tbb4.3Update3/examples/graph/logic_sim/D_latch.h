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

#ifndef __TBBexample_graph_logicsim_dlatch_H
#define __TBBexample_graph_logicsim_dlatch_H 1

#include "basics.h"

class D_latch {
    broadcast_node<signal_t> D_port;
    broadcast_node<signal_t> E_port;
    not_gate a_not;
    and_gate<2> first_and;
    and_gate<2> second_and;
    nor_gate<2> first_nor;
    nor_gate<2> second_nor;
    graph& my_graph;
 public:
    D_latch(graph& g) : my_graph(g), D_port(g), E_port(g), a_not(g), first_and(g), second_and(g), 
                        first_nor(g), second_nor(g) 
    {
        make_edge(D_port, a_not.get_in(0));
        make_edge(D_port, second_and.get_in(1));
        make_edge(E_port, first_and.get_in(1));
        make_edge(E_port, second_and.get_in(0));
        make_edge(a_not.get_out(), first_and.get_in(0));
        make_edge(first_and.get_out(), first_nor.get_in(0));
        make_edge(second_and.get_out(), second_nor.get_in(1));
        make_edge(first_nor.get_out(), second_nor.get_in(0));
        make_edge(second_nor.get_out(), first_nor.get_in(1));
    }
    ~D_latch() {}
    receiver<signal_t>& get_D() { return D_port; }
    receiver<signal_t>& get_E() { return E_port; }
    sender<signal_t>& get_Q() { return first_nor.get_out(); }
    sender<signal_t>& get_notQ() { return second_nor.get_out(); }
};

#endif /* __TBBexample_graph_logicsim_dlatch_H */
