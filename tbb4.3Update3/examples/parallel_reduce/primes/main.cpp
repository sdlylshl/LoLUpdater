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

#include "primes.h"
#include <cstdlib>
#include <cstdio>
#include <cstring>
#include <cctype>
#include <utility>
#include <iostream>
#include <sstream>
#include "tbb/tick_count.h"

#include "../../common/utility/utility.h"

struct RunOptions{
    //! NumberType of threads to use.
    utility::thread_number_range threads;
    //whether to suppress additional output
    bool silentFlag;
    //
    NumberType n;
    //! Grain size parameter
    NumberType grainSize;
    // number of time to repeat calculation
    NumberType repeatNumber;

    RunOptions(utility::thread_number_range threads, NumberType grainSize, NumberType n, bool silentFlag, NumberType repeatNumber)
        : threads(threads), grainSize(grainSize), n(n), silentFlag(silentFlag), repeatNumber(repeatNumber)
    {}
};

int do_get_default_num_threads() {
    int threads;
    #if __TBB_MIC_OFFLOAD
    #pragma offload target(mic) out(threads)
    #endif // __TBB_MIC_OFFLOAD
    threads = tbb::task_scheduler_init::default_num_threads();
    return threads;
}

int get_default_num_threads() {
    static int threads = do_get_default_num_threads();
    return threads;
}

//! Parse the command line.
static RunOptions ParseCommandLine( int argc, const char* argv[] ) {
    utility::thread_number_range threads( get_default_num_threads, 0, get_default_num_threads() );
    NumberType grainSize = 1000;
    bool silent = false;
    NumberType number = 100000000;
    NumberType repeatNumber = 1;

    utility::parse_cli_arguments(argc,argv,
        utility::cli_argument_pack()
            //"-h" option for displaying help is present implicitly
            .positional_arg(threads,"n-of-threads",utility::thread_number_range_desc)
            .positional_arg(number,"number","upper bound of range to search primes in, must be a positive integer")
            .positional_arg(grainSize,"grain-size","must be a positive integer")
            .positional_arg(repeatNumber,"n-of-repeats","repeat the calculation this number of times, must be a positive integer")
            .arg(silent,"silent","no output except elapsed time")
    );

    RunOptions options(threads,grainSize, number, silent, repeatNumber);
    return options;
}

int main( int argc, const char* argv[] ) {
    tbb::tick_count mainBeginMark = tbb::tick_count::now();
    RunOptions options =ParseCommandLine(argc,argv);

    // Try different numbers of threads
    for( int p=options.threads.first; p<=options.threads.last; p=options.threads.step(p) ) {
        for (NumberType i=0; i<options.repeatNumber;++i){
            tbb::tick_count iterationBeginMark = tbb::tick_count::now();
            NumberType count = 0;
            NumberType n = options.n;
            if( p==0 ) {
                #if __TBB_MIC_OFFLOAD
                #pragma offload target(mic) in(n) out(count)
                #endif // __TBB_MIC_OFFLOAD
                count = SerialCountPrimes(n);
            } else {
                NumberType grainSize = options.grainSize;
                #if __TBB_MIC_OFFLOAD
                #pragma offload target(mic) in(n, p, grainSize) out(count)
                #endif // __TBB_MIC_OFFLOAD
                count = ParallelCountPrimes(n, p, grainSize);
            }
            tbb::tick_count iterationEndMark = tbb::tick_count::now();
            if (!options.silentFlag){
                std::cout
                        <<"#primes from [2.." <<options.n<<"] = " << count
                        <<" ("<<(iterationEndMark-iterationBeginMark).seconds()<< " sec with "
                ;
                if( 0 != p )
                    std::cout<<p<<"-way parallelism";
                else
                    std::cout<<"serial code";
                std::cout<<")\n" ;
            }
        }
    }
    utility::report_elapsed_time((tbb::tick_count::now()-mainBeginMark).seconds());
    return 0;
}
