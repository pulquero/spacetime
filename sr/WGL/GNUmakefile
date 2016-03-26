CC = gcc
CFLAGS = -ffast-math -march=pentium
LIBPATH = -L../srel -L../eesr
LIBFLAGS = -lsrel -leesr -lopengl32 -lglu32 -lcomctl32 -lgdi32
LDFLAGS = $(LIBPATH) $(LIBFLAGS)
OBJS = EinsteinExplorer-SR.o

EinsteinExplorer-SR.exe : $(OBJS)
	$(CC) -o EinsteinExplorer-SR.exe $(OBJS) $(LDFLAGS)

EinsteinExplorer-SR.o : EinsteinExplorer-SR.h

all :
	make -C ../srel
	make -C ../eesr
	make

