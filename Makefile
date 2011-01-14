UNAME=$(shell uname)
ifeq ($(UNAME), Linux)
   CFLAGS=-lGL -lglut -g
else
   CFLAGS=-framework GLUT -framework OpenGL -g
endif

PROGNAME=asteroids
FILES=main.cpp Vector3D.cpp GlutUtility.cpp Asteroid3D.cpp AsteroidShip.cpp AsteroidShot.cpp AsteroidShotBeam.cpp

all:
	g++ ${CFLAGS} -Wall -o ${PROGNAME} ${FILES}

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
