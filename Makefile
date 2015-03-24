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
main: matrix.o light.o raytracer.o
	$(CC) $(CFLAGS) raytracer.o matrix.o light.o geometry.o -o raytracer -lfreeimage $(LDFLAGS) 
raytracer.o: raytracer.cpp matrix.o light.o
	$(CC) $(CFLAGS) -c raytracer.cpp -o raytracer.o -lfreeimage
matrix.o: matrix.cpp light.o geometry.o
	$(CC) $(CFLAGS) -c matrix.cpp -o matrix.o
light.o: light.cpp geometry.o
	$(CC) $(CFLAGS) -c light.cpp -o light.o
geometry.o: geometry.cpp
	$(CC) $(CFLAGS) -c geometry.cpp -o geometry.o
clean: 
	$(RM) *.o raytracer output.png
geometry:
	$(CC) $(CFLAGS) -o test test.o $(LDFLAGS) 
