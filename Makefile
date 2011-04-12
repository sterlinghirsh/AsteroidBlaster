# AsteroidBlaster Makefile by Sterling Hirsh
# Uses uname to decide whether it's on Linux or OSX so it can tell which libs to include.

UNAME=$(shell uname)
ifeq ($(UNAME), Linux)
   # Linux stuff
   SDL_LIBS=$(shell "sdl-config" "--libs")
   SDL_CFLAGS=$(shell "sdl-config" "--cflags")
   PLATFORMSPECIFICCFLAGS=-I./Libraries/glew-1.5.8/include -I./Libraries/SDL_ttf-2.0.10 -I./Libraries/SDL_mixer-1.2.11 -I./Libraries/SDL_image-1.2.10
   PLATFORMSPECIFICLDFLAGS= -L./Libraries/glew-1.5.8/lib -L./Libraries/SDL_ttf-2.0.10/.libs -L./Libraries/SDL_mixer-1.2.11/build/.libs/ -L./Libraries/SDL_image-1.2.10/.libs -Wl,-rpath=./Libraries/glew-1.5.8/lib -Wl,-rpath=./Libraries/SDL_ttf-2.0.10/.libs -Wl,-rpath=./Libraries/SDL_mixer-1.2.11/build/.libs/ -Wl,-rpath=./Libraries/SDL_image-1.2.10/.libs  -lGL -lGLU -lSDL -lGLEW -lglut -lpthread
else
   # Mac stuff
   SDL_LIBS=$(shell "/sw/bin/sdl-config" "--libs")
   SDL_CFLAGS=$(shell "/sw/bin/sdl-config" "--cflags")
   PLATFORMSPECIFICCFLAGS=
   PLATFORMSPECIFICLDFLAGS=-framework GLUT -framework OpenGL -Wl,-framework,Cocoa
endif

LDFLAGS=$(PLATFORMSPECIFICLDFLAGS) -g $(SDL_LIBS) -lSDL_mixer -lSDL_image -lSDL_ttf
# -I. -iquote makes it so quoted #includes look in ./
# -Wall makes warnings appear
# -c makes .o files
CFLAGS=$(PLATFORMSPECIFICCFLAGS) -I. -iquote -Wall -c $(SDL_CFLAGS) -g -Wconversion -Werror -O
CC=g++

PROGNAME=AsteroidBlaster

UTILITYFILES=Utility/Vector3D.cpp Utility/GameState.cpp Utility/Custodian.cpp Utility/InputManager.cpp Utility/Point3D.cpp Utility/Radar.cpp Utility/Quaternion.cpp Utility/ViewFrustum.cpp Utility/Matrix4.cpp Utility/GlobalUtility.cpp Utility/Music.cpp Utility/SoundEffect.cpp Utility/Texture.cpp Utility/GameSettings.cpp
MENUFILES=Menus/Menu.cpp Menus/MainMenu.cpp Menus/StoreMenu.cpp Menus/CreditsMenu.cpp Menus/SettingsMenu.cpp Menus/HelpMenu.cpp
GRAPHICSFILES=Graphics/Mesh3D.cpp Graphics/MeshPoint.cpp Graphics/Skybox.cpp Graphics/Sprite.cpp Graphics/Camera.cpp
ITEMSFILES=Items/Drawable.cpp Items/Object3D.cpp Items/Asteroid3D.cpp Items/AsteroidShip.cpp Items/BoundingSpace.cpp Items/Ring.cpp Items/Shard.cpp Items/GlowSquare.cpp Items/BoundingWall.cpp Items/Ball.cpp
SHOTSFILES=Shots/Shot.cpp Shots/BeamShot.cpp Shots/ProjectileShot.cpp Shots/TractorBeamShot.cpp Shots/ElectricityShot.cpp Shots/LawnMowerShot.cpp Shots/RamShot.cpp Shots/AntiInertiaShot.cpp Shots/ExplosiveShot.cpp
AIFILES=AI/FlyingAI.cpp AI/ShootingAI.cpp 
NETWORKFILES=
WEAPONSFILES=Weapons/Blaster.cpp Weapons/RailGun.cpp Weapons/Weapon.cpp Weapons/TractorBeam.cpp Weapons/Electricity.cpp Weapons/LawnMower.cpp Weapons/Ram.cpp Weapons/AntiInertia.cpp Weapons/Bomber.cpp
HUDFILES=HUD/ProgressBar.cpp HUD/Minimap.cpp HUD/Display.cpp
PARTICLEFILES=Particles/Particle.cpp Particles/BlasterShotParticle.cpp Particles/EngineParticle.cpp Particles/BlasterImpactParticle.cpp Particles/TractorAttractionParticle.cpp Particles/ElectricityImpactParticle.cpp Particles/ShardParticle.cpp
TEXTFILES=Text/Text.cpp Text/GameMessage.cpp
FILES=main.cpp $(UTILITYFILES) $(MENUFILES) $(GRAPHICSFILES) $(ITEMSFILES) $(SHOTSFILES) $(AIFILES) $(NETWORKFILES) $(WEAPONSFILES) $(HUDFILES) $(PARTICLEFILES) $(TEXTFILES)

OBJECTS=$(FILES:.cpp=.o)

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
	handin zwood csc476final ${FILES}
	handin zwood csc476finalweb asteroidssite/*

valgrind:
	valgrind --leak-check=full --track-origins=no --log-file=valgrindOutput ./${PROGNAME}

gdb:
	gdb ./${PROGNAME}

clean:
	rm -f *.o */*.o ${PROGNAME} ${PROGNAME}.dSYM

gprof:
	gprof ${PROGNAME} gmon.out | less

killall:
	killall -9 ${PROGNAME}

glc-capture:
	glc-capture -s -o ${PROGNAME}.glc ./${PROGNAME}

glc-play:
	glc-play ${PROGNAME}.glc

glc-encode:
	glc-play ${PROGNAME}.glc -y 1 -o - | mencoder -demuxer y4m - -ovc lavc -lavcopts vcodec=mpeg4:vbitrate=1500 -o video.avi

