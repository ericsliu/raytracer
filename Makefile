CC = g++
ifeq ($(shell sw_vers 2>/dev/null | grep Mac | awk '{ print $$2}'),Mac)
	CFLAGS = -g -DGL_GLEXT_PROTOTYPES -I./include/ -I/usr/X11/include -DOSX
	LDFLAGS = -framework GLUT -framework OpenGL \
    	-L"/System/Library/Frameworks/OpenGL.framework/Libraries" \
    	-lGL -lGLU -lm -lstdc++
else
	CFLAGS = -g -DGL_GLEXT_PROTOTYPES -Iglut-3.7.6-bin
	LDFLAGS = -lglut -lGLU -lGL
endif
	
RM = /bin/rm -f 
all: main
main: light.o raytracer.o
	$(CC) $(CFLAGS) raytracer.o light.o geometry.o -o raytracer $(LDFLAGS) 
raytracer.o: raytracer.cpp light.o
	$(CC) $(CFLAGS) -c raytracer.cpp -o raytracer.o
light.o: light.cpp geometry.o
	$(CC) $(CFLAGS) -c light.cpp -o light.o
geometry.o: geometry.cpp
	$(CC) $(CFLAGS) -c geometry.cpp -o geometry.o
clean: 
	$(RM) *.o raytracer
geometry:
	$(CC) $(CFLAGS) -o test test.o $(LDFLAGS) 
