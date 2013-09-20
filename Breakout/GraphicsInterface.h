#include "Resource.h"
class GraphicsInterface
{
public:

	static GraphicsInterface			*getInterfaceInstance()
	{
		#ifdef _WIN32
		return GraphicsDX11::getInstance();
		#else // ! _WIN32
		return GraphicsOGL4::getInstance();
		#endif
	}

	virtual void clearRenderTarget(float r, float g, float b) = 0;

	virtual int		getTechIDByName(const char *name) = 0;
	virtual void	useTechnique(unsigned int index) = 0;
	virtual void	draw(unsigned int startIndex, unsigned int vertexAmount) = 0;
	virtual void	updateCBOnce(CBOnce cb) = 0;
	virtual void	updateCBCameraMove(CBCameraMove cb) = 0;
	virtual void	updateCBWorld(CBWorld cb) = 0;

	virtual void feedInstanceBuffer( std::vector<PerInstance> perInstance) = 0;
};