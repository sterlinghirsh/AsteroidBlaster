
#include "Graphics/Image.h"
#include <iostream>
#include <map>
#include <string>
#include "SDL_image.h"

std::map<std::string, Image*> Image::images;
int Image::numOfImages = 0;


Image::Image(){
}

Image::~Image() {
   std::map<std::string, Image*>::iterator it = Image::images.begin();
   for (; it != Image::images.end(); ++it ) {
      SDL_FreeSurface(it->second);
   }
}

void Image::Add(int _x, int _y, unsigned int texture, std::string keyName) {
   int x = _x;
   int y = _y;
   unsigned int texture = _texture;

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




