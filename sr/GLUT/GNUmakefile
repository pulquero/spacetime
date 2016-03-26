# Makefile for GNU make

# settings
#WIN32 = YES
#HAVE_LIB3DS = YES
LIB3DS_HOME = ../../../lib3ds-1.2.0
#HAVE_GLUTCAVE = YES
GLUTCAVE_HOME = ../../glutcave

ifdef WIN32
   GLLIB = opengl32
   GLULIB = glu32
   GLUTLIB = glut32
   XINCLUDEPATH =
   XLIBPATH =
   EXEEXT = .exe
else
   GLLIB = GL
   GLULIB = GLU
   GLUTLIB = glut
   XINCLUDEPATH = -I/usr/X11/include
   XLIBPATH = -L/usr/X11/lib
   EXEEXT =
endif

ifdef HAVE_LIB3DS
   CC3DSFLAGS = -I$(LIB3DS_HOME) -DHAVE_LIB3DS
   LD3DSFLAGS = -L$(LIB3DS_HOME)/lib3ds -l3ds
else
   CC3DSFLAGS =
   LD3DSFLAGS =
endif

ifdef HAVE_GLUTCAVE
   CCCAVEFLAGS = -I$(GLUTCAVE_HOME) -DHAVE_GLUTCAVE
else
   CCCAVEFLAGS =
endif

CC = gcc
CFLAGS = -O3 -ffast-math -march=pentium $(XINCLUDEPATH) $(CCCAVEFLAGS) $(CC3DSFLAGS)
LIBPATH = -L../srel -L../eesr $(XLIBPATH)
LIBFLAGS = -lsrel -leesr -l$(GLLIB) -l$(GLULIB) -l$(GLUTLIB)
LDFLAGS = $(LIBPATH) $(LIBFLAGS) $(LD3DSFLAGS)
TARGET = EinsteinExplorer-SR
OBJS = $(TARGET).o

$(TARGET)$(EXEEXT): $(OBJS)
	$(CC) -o $(TARGET)$(EXEEXT) $(OBJS) $(LDFLAGS)

$(TARGET).o: $(TARGET).c $(TARGET).h

all:
	make -C ../srel
	make -C ../eesr
	make

clean:
	rm $(OBJS)
