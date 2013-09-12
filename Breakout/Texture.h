#ifndef _TEXTURE_H_
#define _TEXTURE_H_

#include "FreeImage.h"

class Texture
{
private:

	//image format
	FREE_IMAGE_FORMAT fif;
	//pointer to the image, once loaded
	FIBITMAP *dib;
	//pointer to the image data
	BYTE* bits;
	//image width and height
	unsigned int width, height;

public:
	Texture(void);
	~Texture(void);

	FREE_IMAGE_FORMAT* getFif(){return &fif;}
	void setFif(FREE_IMAGE_FORMAT _fif){fif = _fif;}

	FIBITMAP* getDib(){return dib;}
	void setDib(FIBITMAP* _dib){dib = _dib;}

	BYTE* getBits(){return bits;}
	void setBits(BYTE* _bits){bits = _bits;}

	unsigned int getWidth(){return width;}
	void setWidth(unsigned int _width){width = _width;}

	unsigned int getHeight(){return height;}
	void setHeight(unsigned int _height){height = _height;}
};

#endif