#include "GraphicsDX11.h"


GraphicsDX11::GraphicsDX11()
{
	device						= NULL;
	immediateContext			= NULL;
	renderTargetTex				= NULL;
	renderTargetView			= NULL;
	renderTargetResource		= NULL;
	depthStencilTex				= NULL;
	depthStencilView			= NULL;
	depthStencilResource		= NULL;
	blendOption1				= NULL;
	depthStencilStateEnable		= NULL;
	depthStencilStateDisable	= NULL;
	rasterizerBackface			= NULL;
	rasterizerFrontface			= NULL;
	samplerLinear				= NULL;
}

void GraphicsDX11::init()
{

}

void GraphicsDX11::cleanUp()
{

}

void GraphicsDX11::compileShader()
{

}

void GraphicsDX11::presentSwapChain()
{

}

GraphicsDX11::~GraphicsDX11()
{
	SAFE_DELETE(device);
	SAFE_DELETE(immediateContext);
	SAFE_DELETE(renderTargetTex);
	SAFE_DELETE(renderTargetView);
	SAFE_DELETE(renderTargetResource);
	SAFE_DELETE(depthStencilTex);
	SAFE_DELETE(depthStencilView);
	SAFE_DELETE(depthStencilResource);
	SAFE_DELETE(blendOption1);
	SAFE_DELETE(depthStencilStateEnable);
	SAFE_DELETE(depthStencilStateDisable);
	SAFE_DELETE(rasterizerBackface);
	SAFE_DELETE(rasterizerFrontface);
	SAFE_DELETE(samplerLinear);
}
