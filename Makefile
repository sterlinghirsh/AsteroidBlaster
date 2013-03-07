# AsteroidBlaster Makefile by Sterling Hirsh
# Uses uname to decide whether it's on Linux or OSX so it can tell which libs to include.

FMODLIB_PATH          = Libraries/fmod
# Looks like you have to run this. Not sure why.
export PKG_CONFIG_PATH=/usr/local/lib/pkgconfig/
DOEXPORT:=export PKG_CONFIG_PATH=/usr/local/lib/pkgconfig/
ATSCHOOL:=0
#ATSCHOOL:=1
ATSCHOOL_CFLAGS:=

PROTOBUF_CFLAGS:=
PROTOBUF_LIBS:=
PROTOC_BIN:=protoc

CC:=ccache g++

UNAME:=$(shell uname)
ifeq ($(UNAME), Linux)
   # Linux stuff
   # Use this if you're on a school machine.
	# We have to use this version on linux since the -mt version doesn't exist.
   SDL_LIBS:=$(shell "sdl-config" "--libs") -lSDL_ttf -lSDL_image
   SDL_CFLAGS:=$(shell "sdl-config" "--cflags")
	FMODLIB_NAME_RELEASE:= libfmodex64.so
	#FMODLIB_NAME_RELEASE:= libfmodex.so
	# school
   FMODLIB_NAME_LOGGING  = libfmodexL.so
ifeq ($(ATSCHOOL), 1)
	# school
	ENETINCLUDE:=Libraries/enet-1.3.3/.libs/libenet.a
	GLEWINCLUDE:=
	FMODLIB_NAME_RELEASE:= libfmodex64.so
	PROTOBUF_LIBS:=-lz Libraries/protobuf-2.4.1/src/.libs/libprotobuf.a  Libraries/protobuf-2.4.1/src/.libs/libprotoc.a
	ATSCHOOL_CFLAGS:=-DATSCHOOL -I Libraries/enet-1.3.3/include/ -I Libraries/protobuf-2.4.1/src/
	PROTOC_BIN:=Libraries/protobuf-2.4.1/src/protoc
	CC:=g++
else
	ENETINCLUDE:=-lenet
	GLEWINCLUDE:=-lGLEW
   PROTOBUF_CFLAGS:=$(shell "pkg-config" "--cflags" "protobuf")
   PROTOBUF_LIBS:=-Wl,-Bstatic $(shell "pkg-config" "--libs" "protobuf") -Wl,-Bdynamic
endif

   PLATFORMSPECIFICCFLAGS=-I./Libraries/SDL_ttf-2.0.10 -I./Libraries/SDL_image-1.2.12

   PLATFORMSPECIFICLDFLAGS= -L./Libraries/SDL_ttf-2.0.10/.libs -L./Libraries/SDL_image-1.2.10/.libs -Wl,-rpath=./Libraries/glew-1.5.8/lib -Wl,-rpath=./Libraries/SDL_ttf-2.0.10/.libs -Wl,-rpath=./Libraries/SDL_image-1.2.10/.libs $(FMODLIB_PATH)/$(FMODLIB_NAME_RELEASE)  -lGL -lGLU -lSDL $(GLEWINCLUDE) -lpthread -Wl,-Bstatic $(ENETINCLUDE) -Wl,-Bdynamic
   INSTALL_NAME_TOOL_LINE = 
else
   # Mac stuff
   #SDL_LIBS:=$(shell "/sw/bin/sdl-config" "--static-libs") -lSDL_image -lSDL_ttf 
   #SDL_LIBS:=$(shell "/sw/bin/sdl-config" "--static-libs") /sw/lib/libSDL_image.a /sw/lib/libSDL_ttf.a /sw/lib/libfreetype.a
   #SDL_LIBS:=$(shell "/sw/bin/sdl-config" "--static-libs") /sw/lib/libSDL_image.a /sw/lib/libSDL_ttf.a -lfreetype
   SDL_LIBS:=$(shell "/sw/bin/sdl-config" "--static-libs") -lSDL_image /sw/lib/libSDL_ttf.a -lfreetype
   # We need to handle copying the freetype business.
   SDL_CFLAGS:=$(shell "/sw/bin/sdl-config" "--cflags")
   #SDL_LIBS:=-framework SDL -framework SDL_ttf -framework SDL_image
   #SDL_CFLAGS:=-framework SDL -framework SDL_ttf -framework SDL_image
   PLATFORMSPECIFICCFLAGS=-I /opt/local/include
   PLATFORMSPECIFICLDFLAGS=-framework OpenGL -Wl,-framework,Cocoa -Wl -L$(FMODLIB_PATH) -lfmodex /usr/local/lib/libenet.a
   FMODLIB_NAME_RELEASE  = libfmodex.dylib
   FMODLIB_NAME_LOGGING  = libfmodexL.dylib
   INSTALL_NAME_TOOL_LINE = install_name_tool -change ./${FMODLIB_NAME_RELEASE} ${FMODLIB_PATH}/${FMODLIB_NAME_RELEASE} AsteroidBlaster
   PROTOBUF_CFLAGS:=-I/usr/local/include
   PROTOBUF_LIBS:=-L/usr/local/lib /usr/local/lib/libprotobuf.a -lz
endif

LDFLAGS:=$(PLATFORMSPECIFICLDFLAGS) $(SDL_LIBS) $(PROTOBUF_LIBS) -g -O3 -Wall -Werror
# -I. -iquote makes it so quoted #includes look in ./
# -Wall makes warnings appear
# -c makes .o files


CFLAGS:=$(PLATFORMSPECIFICCFLAGS) -I. -I Libraries/ -c $(SDL_CFLAGS) $(PROTOBUF_CFLAGS) -g -O3 -Wall -Werror $(ATSCHOOL_CFLAGS)

PROGNAME:=AsteroidBlaster
OSX_APP_NAME:=$(PROGNAME).app
BUNDLE_CONTENTS=$(OSX_APP_NAME)/Contents

UTILITYFILES:=Utility/Vector3D.cpp Utility/GameState.cpp Utility/Custodian.cpp Utility/InputManager.cpp Utility/Point3D.cpp Utility/Radar.cpp Utility/Quaternion.cpp Utility/ViewFrustum.cpp Utility/Matrix4.cpp Utility/GlobalUtility.cpp Utility/SoundEffect.cpp Utility/GameSettings.cpp Utility/CollisionTypes.cpp Utility/Timer.cpp

MENUFILES:=Menus/Menu.cpp Menus/MainMenu.cpp Menus/StoreMenu.cpp Menus/CreditsMenu.cpp Menus/SettingsMenu.cpp Menus/HelpMenu.cpp Menus/HighScoreList.cpp

GRAPHICSFILES:=Graphics/Mesh3D.cpp Graphics/MeshPoint.cpp Graphics/Skybox.cpp Graphics/Texture.cpp Graphics/Camera.cpp Graphics/MeshFace.cpp Graphics/Image.cpp

ITEMSFILES:=Items/Drawable.cpp Items/Object3D.cpp Items/Asteroid3D.cpp Items/AsteroidShip.cpp Items/BoundingSpace.cpp Items/Ring.cpp Items/Shard.cpp Items/GlowSquare.cpp Items/BoundingWall.cpp Items/Ball.cpp Items/Spring.cpp

SHOTSFILES:=Shots/Shot.cpp Shots/BeamShot.cpp Shots/BlasterShot.cpp Shots/TractorBeamShot.cpp Shots/ElectricityShot.cpp Shots/ExplosiveShot.cpp Shots/EnergyShot.cpp Shots/TimedBombShot.cpp Shots/RemoteBombShot.cpp Shots/HomingMissileShot.cpp

AIFILES:=AI/FlyingAI.cpp AI/ShootingAI.cpp 

NETWORKFILES:=Network/ClientSide.cpp Network/ServerSide.cpp Network/NetUtility.cpp  Network/gamestate.pb.cc Network/ClientInfo.cpp
# Network/UDP_Server.cpp Network/UDP_Client.cpp Network/ClientNode.cpp Network/NetTimer.cpp Network/NetShard.cpp Network/NetAsteroid.cpp Network/NetShip.cpp Network/NetBlasterShot.cpp Network/NetBeamShot.cpp Network/NetTractorBeamShot.cpp

WEAPONSFILES:=Weapons/Blaster.cpp Weapons/RailGun.cpp Weapons/Weapon.cpp Weapons/TractorBeam.cpp Weapons/Electricity.cpp Weapons/TimedBomber.cpp Weapons/Energy.cpp Weapons/RemoteBomber.cpp Weapons/HomingMissile.cpp Weapons/Ram.cpp


HUDFILES:=HUD/ProgressBar.cpp HUD/Minimap.cpp HUD/Screen.cpp HUD/WeaponDisplay.cpp HUD/ScoreDisplay.cpp

PARTICLEFILES:=Particles/Particle.cpp Particles/BlasterShotParticle.cpp Particles/EngineParticle.cpp Particles/BlasterImpactParticle.cpp Particles/TractorAttractionParticle.cpp Particles/ElectricityImpactParticle.cpp Particles/ShardParticle.cpp

TEXTFILES:=Text/Text.cpp Text/GameMessage.cpp Text/Input.cpp Text/Table.cpp

FILES:=$(UTILITYFILES) $(MENUFILES) $(GRAPHICSFILES) $(ITEMSFILES) $(SHOTSFILES) $(AIFILES) $(NETWORKFILES) $(WEAPONSFILES) $(HUDFILES) $(PARTICLEFILES) $(TEXTFILES)

OBJECTS:=$(FILES:.cpp=.o)
OBJECTS:=$(OBJECTS:.cc=.o)

PROTOBUF_HEADER:=Network/gamestate.pb.h

all: Network/gamestate.pb.h $(FILES) AsteroidBlaster

AsteroidBlaster: AsteroidBlaster.o $(OBJECTS)
	$(CC) AsteroidBlaster.o $(OBJECTS) -o $@ $(LDFLAGS)
	$(INSTALL_NAME_TOOL_LINE)

AsteroidBlaster.o: AsteroidBlaster.cpp
	$(CC) $(CFLAGS) $< -o $@

.cpp.o:
	$(CC) $(CFLAGS) $< -o $@

.cc.o:
	$(CC) $(CFLAGS) $< -o $@

Network/gamestate.pb.h: Network/gamestate.proto
	$(PROTOC_BIN) --cpp_out=./  Network/gamestate.proto


run:
	./${PROGNAME}

runServer:
	./${PROGNAME} -s

runClient:
	./${PROGNAME} -c localhost

testNetworking:
	./${PROGNAME} -s  &
	./${PROGNAME} -c localhost  &
	./${PROGNAME} -c localhost
	

lib:
	./lib.sh make

libClean:
	./lib.sh clean

valgrind:
	/sw/bin/valgrind --leak-check=no --track-origins=yes --log-file=valgrindOutput --dsymutil=yes ./${PROGNAME}

valgrindServer:
	/sw/bin/valgrind --leak-check=no --track-origins=yes --log-file=valgrindOutput --dsymutil=yes ./${PROGNAME} -s

valgrindClient:
	/sw/bin/valgrind --leak-check=no --log-file=valgrindOutput ./${PROGNAME} -c localhost

gdb:
	gdb ./${PROGNAME}

clean:
	rm -rf *.o */*.o ${PROGNAME} ${PROGNAME}.dSYM Client Server

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

osx:
	rm -r osxresult/
	mkdir -p $(BUNDLE_CONTENTS)/MacOS
	mkdir -p $(BUNDLE_CONTENTS)/Resources
	mkdir -p $(BUNDLE_CONTENTS)/Frameworks
	cp Libraries/fmod/libfmodex.dylib $(BUNDLE_CONTENTS)/Frameworks
	echo "AAPL????" > $(BUNDLE_CONTENTS)/PkgInfo
	cp $(PROGNAME) $(BUNDLE_CONTENTS)/MacOS/
	cp -r Fonts $(BUNDLE_CONTENTS)/
	cp -r Images $(BUNDLE_CONTENTS)/
	cp -r Shaders $(BUNDLE_CONTENTS)/
	cp -r Sounds $(BUNDLE_CONTENTS)/
	install_name_tool -change $(FMODLIB_PATH)/$(FMODLIB_NAME_RELEASE) @executable_path/../Frameworks/$(FMODLIB_NAME_RELEASE) $(BUNDLE_CONTENTS)/MacOS/$(PROGNAME)
	mkdir osxresult
	mv $(OSX_APP_NAME) osxresult/

runOsx:
	open osxresult/$(OSX_APP_NAME)
