#!/bin/bash

if [ "$1" == "make" ] 
then
   lib.sh clean
   #get into Libraries directory
   cd Libraries

   #SDL_ttf--------------------
   echo "SDL_ttf"
   echo "Unzipping.."
   gunzip SDL_ttf-2.0.10.tar.gz
   echo "Extracting.."
   tar -xf SDL_ttf-2.0.10.tar
   #zipping the .tar file back up
   gzip SDL_ttf-2.0.10.tar
   cd SDL_ttf-2.0.10
   echo "Configuring.."
   ./configure
   echo "Making.."
   make
   echo "Done!"
   cd ..

   #glew--------------------
   echo "GLEW"
   echo "Unzipping.."
   gunzip glew-1.5.8.tar.gz
   echo "Extracting.."
   tar -xf glew-1.5.8.tar
   #zipping the .tar file back up
   gzip glew-1.5.8.tar
   cd glew-1.5.8
   echo "Making.."
   make
   echo "Done!"
   cd ..
   
   #go back to parent directory
   cd ..
   
elif [ "$1" == "clean" ] 
then
   cd Libraries

   #SDL_ttf--------------------
   echo "SDL_ttf"
   echo "Cleaning..."
   rm -rf SDL_ttf-2.0.10
   
   #glew--------------------
   echo "GLEW"
   echo "Cleaning..."
   rm -rf glew-1.5.8
else
   echo "Usage: lib [make | clean]"
fi

exit 0
