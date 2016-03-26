LDFLAGS = -lglut -lGLU -lGL -lXmu -lXi -lXext -lX11 -lm -L/usr/X11R6/lib
OBJECTS = main.o street_data.o render.o
CFLAGS = -g

street: $(OBJECTS)
	$(CC) -o $@ $(OBJECTS) $(LDFLAGS)

clean:
	rm -f *.o street

package: clean
	( cd .. && tar czvf street.tar.gz street )

