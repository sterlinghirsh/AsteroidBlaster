# AsteroidBlaster Makefile by Sterling Hirsh
# Uses uname to decide whether it's on Linux or OSX so it can tell which libs to include.

UNAME=$(shell uname)
ifeq ($(UNAME), Linux)
   # Linux stuff
   SDL_LIBS=$(shell "sdl-config" "--libs")
   SDL_CFLAGS=$(shell "sdl-config" "--cflags")
   PLATFORMSPECIFICCFLAGS=-I./Libraries/glew-1.5.8/include -I./Libraries/SDL_ttf-2.0.10 -I./Libraries/SDL_mixer-1.2.11 -I./Libraries/SDL_image-1.2.10
   PLATFORMSPECIFICLDFLAGS= -L./Libraries/glew-1.5.8/lib -L./Libraries/SDL_ttf-2.0.10/.libs -L./Libraries/SDL_mixer-1.2.11/build/.libs/ -L./Libraries/SDL_image-1.2.10/.libs -Wl,-rpath=./Libraries/glew-1.5.8/lib -Wl,-rpath=./Libraries/SDL_ttf-2.0.10/.libs -Wl,-rpath=./Libraries/SDL_mixer-1.2.11/build/.libs/ -Wl,-rpath=./Libraries/SDL_image-1.2.10/.libs  -lGL -lGLU -lSDL -lSDL_ttf -lSDL_image -lGLEW -lglut -lpthread
else
   # Mac stuff
   SDL_LIBS=$(shell "/sw/bin/sdl-config" "--libs")
   SDL_CFLAGS=$(shell "/sw/bin/sdl-config" "--cflags")
   PLATFORMSPECIFICCFLAGS=
   PLATFORMSPECIFICLDFLAGS=-framework GLUT -framework OpenGL -Wl,-framework,Cocoa
endif

LDFLAGS=$(PLATFORMSPECIFICLDFLAGS) -g $(SDL_LIBS) -lSDL_mixer
# -I. -iquote makes it so quoted #includes look in ./
# -Wall makes warnings appear
# -c makes .o files
CFLAGS=$(PLATFORMSPECIFICCFLAGS) -I. -iquote -Wall -c $(SDL_CFLAGS) -g -O0
CC=g++

PROGNAME=AsteroidBlaster

UTILITYFILES=Utility/Vector3D.cpp Utility/BitmapTextDisplay.cpp Utility/GameState.cpp Utility/Custodian.cpp Utility/InputManager.cpp Utility/Point3D.cpp Utility/Radar.cpp Utility/Quaternion.cpp Utility/ViewFrustum.cpp Utility/Matrix4.cpp Utility/GlobalUtility.cpp Utility/Menu.cpp Utility/Music.cpp Utility/SoundEffect.cpp Utility/Image.cpp
GRAPHICSFILES=Graphics/Mesh3D.cpp Graphics/MeshPoint.cpp Graphics/Skybox.cpp Graphics/Sprite.cpp Graphics/TextureImporter.cpp Graphics/MeshImporter.cpp Graphics/Camera.cpp 
ITEMSFILES=Items/Drawable.cpp Items/Object3D.cpp Items/Asteroid3D.cpp Items/AsteroidShip.cpp Items/BoundingSpace.cpp Items/BoundingSphere.cpp Items/Player.cpp Items/Enemy.cpp Items/Ring.cpp Items/Shard.cpp Items/GlowSquare.cpp Items/BoundingWall.cpp
SHOTSFILES=Shots/Shot.cpp Shots/BeamShot.cpp Shots/ProjectileShot.cpp Shots/TractorBeamShot.cpp Shots/ElectricityShot.cpp Shots/LawnMowerShot.cpp
AIFILES=AI/FlyingAI.cpp AI/ShootingAI.cpp 
NETWORKFILES=
WEAPONSFILES=Weapons/Blaster.cpp Weapons/RailGun.cpp Weapons/Weapon.cpp Weapons/TractorBeam.cpp Weapons/Electricity.cpp Weapons/LawnMower.cpp
HUDFILES=HUD/ProgressBar.cpp HUD/Minimap.cpp
PARTICLEFILES=Particles/Particle.cpp Particles/BlasterShotParticle.cpp
FILES=main.cpp $(UTILITYFILES) $(GRAPHICSFILES) $(ITEMSFILES) $(SHOTSFILES) $(AIFILES) $(NETWORKFILES) $(WEAPONSFILES) $(HUDFILES) $(PARTICLEFILES)

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

lib:
	./lib.sh make

libClean:
	./lib.sh clean

turnin:
	handin zwood csc471finals2 *.cpp *.h *.bmp README
	handin zwood csc471finalwebs2 asteroidssite/*
	handin zwood csc471finals2
	handin zwood csc471finalwebs2

valgrind:
	valgrind -v --track-origins=yes ./${PROGNAME}

gdb:
	gdb ./${PROGNAME}

clean:
	rm -f *.o */*.o ${PROGNAME} ${PROGNAME}.dSYM

gprof:
	gprof ${PROGNAME} gmon.out | less

killall:
	killall -9 ${PROGNAME}
