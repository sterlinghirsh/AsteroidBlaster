# AsteroidBlaster Makefile by Sterling Hirsh
# Uses uname to decide whether it's on Linux or OSX so it can tell which libs to include.

UNAME=$(shell uname)
ifeq ($(UNAME), Linux)
   PLATFORMSPECIFICCFLAGS=
   PLATFORMSPECIFICLDFLAGS=-lGL -lGLU -lglut -g
else
   PLATFORMSPECIFICCFLAGS=
   PLATFORMSPECIFICLDFLAGS=-framework GLUT -framework OpenGL -g
endif

LDFLAGS=$(PLATFORMSPECIFICLDFLAGS)

# -I. -iquote makes it so quoted #includes look in ./
# -Wall makes warnings appear
# -c makes .o files
CFLAGS=$(PLATFORMSPECIFICCFLAGS) -I. -iquote -Wall -c
CC=g++

PROGNAME=AsteroidBlaster

FILES=main.cpp Utility/Vector3D.cpp Graphics/GlutUtility.cpp Items/Asteroid3D.cpp Items/AsteroidShip.cpp Shots/AsteroidShot.cpp Shots/AsteroidShotBeam.cpp Graphics/Mesh3D.cpp Graphics/MeshPoint.cpp Graphics/Skybox.cpp Graphics/Sprite.cpp Graphics/TextureImporter.cpp

OBJECTS=$(FILES:.cpp=.o)

#all:
#	g++ ${CFLAGS} -o ${PROGNAME} ${FILES}

all: $(FILES) $(PROGNAME)

$(PROGNAME): $(OBJECTS)
	$(CC) $(LDFLAGS) $(OBJECTS) -o $@

.cpp.o:
	$(CC) $(CFLAGS) $< -o $@


run:
	./${PROGNAME}

turnin:
	handin zwood csc471finals2 *.cpp *.h *.bmp README
	handin zwood csc471finalwebs2 asteroidssite/*
	handin zwood csc471finals2
	handin zwood csc471finalwebs2

valgrind:
	valgrind ./${PROGNAME}

gdb:
	gdb ./${PROGNAME}

clean:
	rm -f *.o */*.o ${PROGNAME} ${PROGNAME}.dSYM
