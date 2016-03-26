# Makefile for GNU make

ifdef WIN32
   OPTIMIZE = -O3 -ffast-math -march=pentium
   XINCLUDEPATH =
else
   OPTIMIZE = -ffast-math
   XINCLUDEPATH = -I/usr/X11/include
endif
CC = gcc
CFLAGS = $(OPTIMIZE) $(XINCLUDEPATH)
OBJS = eesr.o

all: libeesr.a

libeesr.a: $(OBJS)
	ar rs libeesr.a $(OBJS)

eesr.o: eesr.h _eesr.h

