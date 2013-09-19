#include "Texture.h"
#include "Resource.h"

namespace Resources
{

	Texture::Texture()
	{
		fif = FIF_UNKNOWN;
		dib = 0;
		bits = 0;
		width = 0;
		height = 0;

		filePath = 0;
	}

	Texture::~Texture()
	{
	}
}