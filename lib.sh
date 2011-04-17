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
   cd SDL_ttf-2.0.10
   echo "Configuring.."
   ./configure || exit
   echo "Making.."
   make -j || exit
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
   cd SDL_mixer-1.2.11
   echo "Configuring.."
   ./configure || exit
   echo "Making.."
   make -j || exit
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
   cd SDL_image-1.2.10
   echo "Configuring.."
   ./configure || exit
   echo "Making.."
   make -j || exit
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
   cd glew-1.5.8
   echo "Making.."
   make -j || exit
   echo "Done!"
   cd ..
   
   #go back to parent directory
   cd ..
   svn revert -R Libraries
   
elif [ "$1" == "clean" ] 
then
   cd Libraries

   #SDL_ttf--------------------
   echo "SDL_ttf"
   echo "Cleaning..."
   rm -rf SDL_ttf-2.0.10
   
   #SDL_mixer--------------------
   echo "SDL_ttf"
   echo "Cleaning..."
   rm -rf SDL_mixer-1.2.11
   
   #SDL_image--------------------
   echo "SDL_ttf"
   echo "Cleaning..."
   rm -rf SDL_image-1.2.10
   
   #glew--------------------
   echo "GLEW"
   echo "Cleaning..."
   rm -rf glew-1.5.8
else
   echo "Usage: lib [make | clean]"
fi

exit 0
