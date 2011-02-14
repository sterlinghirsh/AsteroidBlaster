#!/bin/bash

if [ "$1" == "make" ] 
then
   #get into Libraries directory
   cd Libraries

   #SDL_ttf--------------------


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
   
   #glew--------------------
   echo "GLEW"
   echo "Cleaning..."
   rm -rf glew-1.5.8
else
   echo "Usage: lib [make | clean]"
fi

exit 0
