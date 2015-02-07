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

#ifndef TREE_MAKER_H_
#define TREE_MAKER_H_

#include "tbb/tick_count.h"
#include "tbb/task.h"

static double Pi = 3.14159265358979;

const bool tbbmalloc = true;
const bool stdmalloc = false;

template<bool use_tbbmalloc>
class TreeMaker {

    class SubTreeCreationTask: public tbb::task {
        TreeNode*& my_root;
        bool is_continuation;
        typedef TreeMaker<use_tbbmalloc> MyTreeMaker;

    public:
        SubTreeCreationTask( TreeNode*& root, long number_of_nodes ) : my_root(root), is_continuation(false) {
            my_root = MyTreeMaker::allocate_node();
            my_root->node_count = number_of_nodes;
            my_root->value = Value(Pi*number_of_nodes);
        }

        tbb::task* execute() {
            tbb::task* next = NULL;
            if( !is_continuation ) {
                long subtree_size = my_root->node_count - 1;
                if( subtree_size<1000 ) { /* grainsize */
                    my_root->left  = MyTreeMaker::do_in_one_thread(subtree_size/2);
                    my_root->right = MyTreeMaker::do_in_one_thread(subtree_size - subtree_size/2);
                } else {
                    // Create tasks before spawning any of them.
                    tbb::task* a = new( allocate_child() ) SubTreeCreationTask(my_root->left,subtree_size/2);
                    tbb::task* b = new( allocate_child() ) SubTreeCreationTask(my_root->right,subtree_size - subtree_size/2);
                    recycle_as_continuation();
                    is_continuation = true;
                    set_ref_count(2);
                    spawn(*b);
                    next = a;
                }
            } 
            return next;
        }
    };

public:
    static TreeNode* allocate_node() {
        return use_tbbmalloc? tbb::scalable_allocator<TreeNode>().allocate(1) : new TreeNode;
    }

    static TreeNode* do_in_one_thread( long number_of_nodes ) {
        if( number_of_nodes==0 ) {
            return NULL;
        } else {
            TreeNode* n = allocate_node();
            n->node_count = number_of_nodes;
            n->value = Value(Pi*number_of_nodes);
            --number_of_nodes;
            n->left  = do_in_one_thread( number_of_nodes/2 ); 
            n->right = do_in_one_thread( number_of_nodes - number_of_nodes/2 );
            return n;
        }
    }

    static TreeNode* do_in_parallel( long number_of_nodes ) {
        TreeNode* root_node;
        SubTreeCreationTask& a = *new(tbb::task::allocate_root()) SubTreeCreationTask(root_node, number_of_nodes);
        tbb::task::spawn_root_and_wait(a);
        return root_node;
    }

    static TreeNode* create_and_time( long number_of_nodes, bool silent=false ) {
        tbb::tick_count t0, t1;
        TreeNode* root = allocate_node();
        root->node_count = number_of_nodes;
        root->value = Value(Pi*number_of_nodes);
        --number_of_nodes;

        t0 = tbb::tick_count::now();
        root->left  = do_in_one_thread( number_of_nodes/2 );
        t1 = tbb::tick_count::now();
        if ( !silent ) printf ("%24s: time = %.1f msec\n", "half created serially", (t1-t0).seconds()*1000);

        t0 = tbb::tick_count::now();
        root->right = do_in_parallel( number_of_nodes - number_of_nodes/2 );
        t1 = tbb::tick_count::now();
        if ( !silent ) printf ("%24s: time = %.1f msec\n", "half done in parallel", (t1-t0).seconds()*1000);

        return root;
    }
};

#endif // TREE_MAKER_H_
