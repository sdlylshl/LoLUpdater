# Copyright 2005-2015 Intel Corporation.  All Rights Reserved.
#
# The source code contained or described herein and all documents related
# to the source code ("Material") are owned by Intel Corporation or its
# suppliers or licensors.  Title to the Material remains with Intel
# Corporation or its suppliers and licensors.  The Material is protected
# by worldwide copyright laws and treaty provisions.  No part of the
# Material may be used, copied, reproduced, modified, published, uploaded,
# posted, transmitted, distributed, or disclosed in any way without
# Intel's prior express written permission.
#
# No license under any patent, copyright, trade secret or other
# intellectual property right is granted to or conferred upon you by
# disclosure or delivery of the Materials, either expressly, by
# implication, inducement, estoppel or otherwise.  Any license under such
# intellectual property rights must be express and approved by Intel in
# writing.

# Common Makefile that builds and runs examples.
PROG=sub_string_finder_extended
ARGS=
LIGHT_PROG=sub_string_finder

# The C++ compiler options
# Trying to find if icl.exe is set
CXX1 = $(TBB_CXX)-
CXX2 = $(CXX1:icl.exe-=icl.exe)
CXX  = $(CXX2:-=cl.exe)

MYCXXFLAGS = /TP /EHsc /W3 /nologo /D _CONSOLE /D _MBCS /D WIN32 $(offload:mic=/D__TBB_MIC_OFFLOAD /Qdiag-disable:3335,3440,3441) $(CXXFLAGS)
MYLDFLAGS =/INCREMENTAL:NO /NOLOGO /DEBUG /FIXED:NO $(LDFLAGS)

TBB_MIC_LIB = $(offload:mic=/Qtbb)
TBB_DEBUG_MIC_LIB = $(offload:mic=/Qoffload-option,mic,link,"-ltbb_debug")

all: release test
release: compiler_check
	$(CXX) sub_string_finder.cpp /MD /O2 /D NDEBUG $(MYCXXFLAGS) $(TBB_MIC_LIB) /link tbb.lib $(LIBS) $(MYLDFLAGS) /OUT:sub_string_finder.exe
	$(CXX) sub_string_finder_extended.cpp /MD /O2 /D NDEBUG $(MYCXXFLAGS) $(TBB_MIC_LIB) /link tbb.lib $(LIBS) $(MYLDFLAGS) /OUT:sub_string_finder_extended.exe
	$(CXX) sub_string_finder_pretty.cpp /MD /O2 /D NDEBUG $(MYCXXFLAGS) $(TBB_MIC_LIB) /link tbb.lib $(LIBS) $(MYLDFLAGS) /OUT:sub_string_finder_pretty.exe
debug: compiler_check
	$(CXX) sub_string_finder.cpp /MDd /Od /Zi /D TBB_USE_DEBUG /D _DEBUG $(MYCXXFLAGS) $(TBB_DEBUG_MIC_LIB) /link tbb_debug.lib $(LIBS) $(MYLDFLAGS) /OUT:sub_string_finder.exe
	$(CXX) sub_string_finder_extended.cpp /MDd /Od /Zi /D TBB_USE_DEBUG /D _DEBUG $(MYCXXFLAGS) $(TBB_DEBUG_MIC_LIB) /link tbb_debug.lib $(LIBS) $(MYLDFLAGS) /OUT:sub_string_finder_extended.exe
	$(CXX) sub_string_finder_pretty.cpp /MDd /Od /Zi /D TBB_USE_DEBUG /D _DEBUG $(MYCXXFLAGS) $(TBB_DEBUG_MIC_LIB) /link tbb_debug.lib $(LIBS) $(MYLDFLAGS) /OUT:sub_string_finder_pretty.exe
clean:
	@cmd.exe /C del sub_string_finder*.exe *.obj *.?db *.manifest
test:
	$(PROG) $(ARGS)
light_test:
	$(LIGHT_PROG) $(ARGS)
compiler_check:
	@echo compiler_test>compiler_test && @$(CXX) /E compiler_test >nul 2>&1  || echo "$(CXX) command not found. Check if CXX=$(CXX) is set properly"
	@cmd.exe /C del compiler_test
