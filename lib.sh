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
   git checkout -- SDL_ttf-2.0.10.tar.gz
   cd SDL_ttf-2.0.10
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
   git checkout -- SDL_image-1.2.10.tar.gz
   cd SDL_image-1.2.10
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
   git checkout -- glew-1.5.8.tar.gz
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
   
   #SDL_image--------------------
   echo "Cleaning SDL_image..."
   rm -rf SDL_image-1.2.10
   
   #glew--------------------
   echo "Cleaning GLEW..."
   rm -rf glew-1.5.8
   git checkout -- * 
else
   echo "Usage: lib [make | clean]"
fi

exit 0
