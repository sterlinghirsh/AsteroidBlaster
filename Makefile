UNAME=$(shell uname)
ifeq ($(UNAME), Linux)
   CFLAGS=-lGL -lGLU -lglut -g
else
   CFLAGS=-framework GLUT -framework OpenGL -g
endif

PROGNAME=asteroids

FILES=main.cpp Utility/Vector3D.cpp Graphics/GlutUtility.cpp Items/Asteroid3D.cpp Items/AsteroidShip.cpp Shots/AsteroidShot.cpp Shots/AsteroidShotBeam.cpp Graphics/Mesh3D.cpp Graphics/MeshPoint.cpp Graphics/Skybox.cpp Graphics/Sprite.cpp Graphics/TextureImporter.cpp

all:
	g++ ${CFLAGS} -I. -iquote -Wall -o ${PROGNAME} ${FILES}

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
