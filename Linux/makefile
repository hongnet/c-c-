#
# CopyRight: .
# Desc:      .
# Date:      
# Contact:   
# History:   
# Useage:    make

#PC Version(32 or 64)
include ./Rules.make

#Modify -I and -L options in different projects
CPPFLAGS = -I. \
           -I../include \
		   -I../include/hpr \
		   -I../include/utils \
		   -I../include/hplug \

ifeq ($(PCVersion),64)
LIBS_DIR1 = ../bin/x64/$(OSVersion)
endif

LIBS_DIR = $(LIBS_DIR1)/
CPPFLAGS += -D_REENTRANT -std=c++11
LDFLAGS += -L$(LIBS_DIR1) -L$(LIBS_DIR) -Wl,-rpath=./hplugin/datasrc/ga1400_trans_reg_sh1400directProtocol



#Link the .so or .a library
LIBS = -lhpr -lhlog -ljson_linux-gcc-4.1.2_libmt -lhttpsdk -lrdkafka -lrdkafka++ -lz

#Target file
TARGETSO = libga1400_trans_reg_sh1400directProtocol.so

#Source and obj file
SRCS = $(wildcard *.c) $(wildcard *.cc ../include/json/*.cc  ../include/base64/3rdParty/*.cc ../include/base64/*.cc ../include//MD5/*.cc) $(wildcard *.cpp ../include/json/*.cpp ../include/librdkafka/OperateKafka.cpp ../include/MakeUpXml/Markup.cpp ../include/base64/3rdParty/*.cpp ../include/base64/*.cpp ../include//MD5/*.cpp) $(wildcard *.cxx ../include/json/*.cxx ../include/base64/3rdParty/*.cxx ../include/base64/*.cxx ../include//MD5/*.cxx)
OBJS = $(addsuffix .o, $(basename $(SRCS)))

#Gernation the target file
all: $(TARGETSO)

$(TARGETSO): $(OBJS)
	mkdir -p $(LIBS_DIR)
	$(CXX) $(OPTS) $(LDFLAGS) -o $@ $^ $(LIBS)
	cp $@ $(LIBS_DIR) -rf

%.o : %.c
	$(CC) -c $(OPTS) $(CPPFLAGS) $(CFLAGS) $< -o $@

%.o : %.cc
	$(CXX) -c $(OPTS) $(CPPFLAGS) $(CXXFLAGS) $< -o $@

%.o : %.cpp
	$(CXX) -c $(OPTS) $(CPPFLAGS) $(CXXFLAGS) $< -o $@

%.o : %.cxx
	$(CXX) -c $(OPTS) $(CPPFLAGS) $(CXXFLAGS) $< -o $@

clean:
	-rm $(OBJS) $(TARGETSO)
	-rm -f *~
