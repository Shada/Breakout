#include "Texture.h"

namespace Resources
{

	Texture::Texture(void)
	{
		fif = FIF_UNKNOWN;
		dib = 0;
		bits = 0;
		width = 0;
		height = 0;
	}


	Texture::~Texture(void)
	{
	}

}