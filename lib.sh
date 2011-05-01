#!/bin/bash

if [ "$1" == "make" ] 
then
   ./lib.sh clean
   #get into Libraries directory
   cd Libraries

   #SDL_ttf--------------------
   echo "SDL_ttf"
   echo "Unzipping.."
   gunzip SDL_ttf-2.0.10.tar.gz
   echo "Extracting.."
   tar -xf SDL_ttf-2.0.10.tar
   #deleting gunziped file
   rm SDL_ttf-2.0.10.tar
   svn revert SDL_ttf-2.0.10.tar.gz
   cd SDL_ttf-2.0.10
   echo "Configuring.."
   ./configure || exit
   echo "Making.."
   make -j 3 || exit
   echo "Done!"
   cd ..
   
   #SDL_mixer--------------------
   echo "SDL_mixer"
   echo "Unzipping.."
   gunzip SDL_mixer-1.2.11.tar.gz
   echo "Extracting.."
   tar -xf SDL_mixer-1.2.11.tar
   #deleting gunziped file
   rm SDL_mixer-1.2.11.tar
   svn revert SDL_mixer-1.2.11.tar.gz
   cd SDL_mixer-1.2.11
   echo "Configuring.."
   ./configure || exit
   echo "Making.."
   make -j 3 || exit
   echo "Done!"
   cd ..
   
   #SDL_image--------------------
   echo "SDL_image"
   echo "Unzipping.."
   gunzip SDL_image-1.2.10.tar.gz
   echo "Extracting.."
   tar -xf SDL_image-1.2.10.tar
   #deleting gunziped file
   rm SDL_image-1.2.10.tar
   svn revert SDL_image-1.2.10.tar.gz
   cd SDL_image-1.2.10
   echo "Configuring.."
   ./configure || exit
   echo "Making.."
   make -j 3 || exit
   echo "Done!"
   cd ..
   
   #SDL_net--------------------
   echo "SDL_net"
   echo "Unzipping.."
   gunzip SDL_net-1.2.7.tar.gz
   echo "Extracting.."
   tar -xf SDL_net-1.2.7.tar
   #deleting gunziped file
   rm SDL_net-1.2.7.tar
   svn revert SDL_net-1.2.7.tar.gz
   cd SDL_net-1.2.7
   echo "Configuring.."
   ./configure || exit
   echo "Making.."
   make -j 3 || exit
   echo "Done!"
   cd ..

   #glew--------------------
   echo "GLEW"
   echo "Unzipping.."
   gunzip glew-1.5.8.tar.gz
   echo "Extracting.."
   tar -xf glew-1.5.8.tar
   #deleting gunziped file
   rm glew-1.5.8.tar
   svn revert glew-1.5.8.tar.gz
   cd glew-1.5.8
   echo "Making.."
   make -j 3 || exit
   echo "Done!"
   cd ..
   
elif [ "$1" == "clean" ] 
then
   cd Libraries
   
   #SDL_ttf--------------------
   echo "Cleaning SDL_ttf..."
   rm -rf SDL_ttf-2.0.10
   
   #SDL_mixer--------------------
   echo "Cleaning SDL_mixer..."
   rm -rf SDL_mixer-1.2.11
   
   #SDL_image--------------------
   echo "Cleaning SDL_image..."
   rm -rf SDL_image-1.2.10
   
   #SDL_net--------------------
   echo "Cleaning SDL_net..."
   rm -rf SDL_net-1.2.7
   
   #glew--------------------
   echo "Cleaning GLEW..."
   rm -rf glew-1.5.8
   svn revert * 
else
   echo "Usage: lib [make | clean]"
fi

exit 0
