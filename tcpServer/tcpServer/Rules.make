#################################
#Debug or Release, Debug: -g Release: -O3
OPTS = -g -O2

#the version of os£¬linux/suse
OSVersion = linux
PCVersion = 64

#################################
ifeq ($(OSVersion),suse)
LDFLAGS = -shared -fPIC -Wl,-Bsymbolic,-rpath=./ -Wl,--hash-style=sysv
endif

ifeq ($(OSVersion),linux)
LDFLAGS = -shared -fPIC -Wl,-Bsymbolic,-rpath=./ -Wl,--hash-style=gnu
endif

#################################
# gcc compiler
ifeq ($(PCVersion),64)
CC = gcc
endif

ifeq ($(PCVersion),32)
CC = gcc -m32
endif

#CFLAGS = -Wall -Wno-strict-aliasing -Wno-unused-variable
CFLAGS = $(OPTS) -fPIC -Wall -Wno-strict-aliasing -Wno-unused-variable

#################################
# g++ compiler
ifeq ($(PCVersion),64)
CXX  = g++
endif

ifeq ($(PCVersion),32)
CXX  = g++ -m32
endif

#CXXFLAGS = -Wall -Wno-strict-aliasing -Wno-unused-variable
CXXFLAGS = $(OPTS) -fPIC -Wall -Wno-strict-aliasing -Wno-unused-variable

#################################
#Static library
AR = ar
ARFLAGS = rs
