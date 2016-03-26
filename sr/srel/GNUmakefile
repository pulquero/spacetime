# Makefile for GNU make

ifdef WIN32
   OPTIMIZE = -ffast-math -fthread-jumps -fdefer-pop -fcaller-saves -fcse-follow-jumps -fcse-skip-blocks -fregmove -fexpensive-optimizations -march=pentium
   XINCLUDEPATH =
else
   OPTIMIZE = -ffast-math -fthread-jumps -fdefer-pop -fcaller-saves -fcse-follow-jumps -fcse-skip-blocks -fregmove -fexpensive-optimizations
   XINCLUDEPATH = -I/usr/X11/include
endif
CC = gcc
CFLAGS = $(OPTIMIZE) $(XINCLUDEPATH)
OBJS = srel.o

all: libsrel.a

libsrel.a: $(OBJS)
	ar rs libsrel.a $(OBJS)

srel.o: srel.h _srel.h

