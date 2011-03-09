
#include "Utility/Image.h"
#include <iostream>
#include <map>
#include "SDL_image.h"

std::map<std::string, SDL_Surface*> Image::images;
int Image::numOfImages = 0;


Image::Image() {

}

Image::~Image() {
   std::map<std::string, SDL_Surface*>::iterator it = Image::images.begin();
   for (; it != Image::images.end(); ++it ) {
      SDL_FreeSurface(it->second);
   }
}

void Image::Add(std::string file, std::string keyName) {
   SDL_Surface *image;
   
   
	image=IMG_Load(file.c_str());
	if(!image)
	{
		/* image failed to load */
		printf("IMG_Load: %s\n", IMG_GetError());
		SDL_Quit();
		exit(1);
	}
	
   
   images.insert( std::pair<std::string, SDL_Surface*>(keyName,image) );
   numOfImages++;
}


/**
 * loop defaults to false.
 */
SDL_Surface* Image::getImage(std::string keyName) {
   std::map<std::string, SDL_Surface*>::iterator iter = images.find(keyName);

   if (iter == images.end()) {
      std::cerr << "failed to find: " << keyName << " in Image::images." << std::endl;
      exit(1);
   }
   
   return iter->second;
}



