BUILD_HOME   := $(shell dirname `cd ../; pwd`)
Project      := xhal
Package      := xhal
ShortPackage := xhal
LongPackage  := xhalarm
PackageName  := $(ShortPackage)
PackagePath  := $(shell pwd)
PackageDir   := pkg/$(ShortPackage)
Packager     := Mykhailo Dalchenko
Arch         := arm

XHAL_VER_MAJOR=1
XHAL_VER_MINOR=0
XHAL_VER_PATCH=0

include $(BUILD_HOME)/$(Project)/config/mfZynq.mk
include $(BUILD_HOME)/$(Project)/config/mfCommonDefs.mk
include $(BUILD_HOME)/$(Project)/config/mfRPMRules.mk

ADDFLAGS=-std=gnu++14

IncludeDirs  = ${BUILD_HOME}/${Project}/xhalcore/include
INC=$(IncludeDirs:%=-I%)

Libraries+= -llog4cplus -lxerces-c -lstdc++
LIB=$(LibraryDirs)
LIB+= $(Libraries)

LDFLAGS= -shared
SRCS_XHAL = $(shell echo ../xhalcore/src/common/utils/*.cpp)
OBJS_XHAL = $(SRCS_XHAL:.cpp=.o)

XHAL_LIB=${BUILD_HOME}/${Project}/${LongPackage}/lib/libxhal.so

.PHONY: clean rpc prerpm

default: clean
	@echo "Running default target"
	$(MakeDir) $(PackageDir)

_rpmprep: preprpm
	@echo "Running _rpmprep target"
preprpm: default
	@echo "Running preprpm target"
	@cp -rf lib $(PackageDir)

build:${XHAL_LIB}

_all: clean ${XHAL_LIB} 

$(XHAL_LIB): $(OBJS_XHAL) 
	@mkdir -p ${BUILD_HOME}/${Project}/${LongPackage}/lib
	$(CC) $(CFLAGS) $(ADDFLAGS) ${LDFLAGS} $(INC) $(LIB) -o $@ $^

$(OBJS_XHAL):$(SRCS_XHAL)
	$(CC) $(CFLAGS) $(ADDFLAGS) $(INC) $(LIB) -c -o $@ $<

%.o: %.c
	$(CC) -std=gnu99 -c $(CFLAGS) -o $@ $<
%.o: %.cc
	$(CXX) -std=c++0x -c $(CFLAGS) -o $@ $<

clean: cleanrpm
	-${RM} ${XHAL_LIB} ${OBJS_XHAL}
	-rm -rf $(PackageDir)

cleandoc: 
	@echo "TO DO"