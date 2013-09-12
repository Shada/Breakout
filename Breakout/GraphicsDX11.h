#ifdef _WIN32

#pragma once
#include "graphicsinterface.h"
#include <d3dcompiler.h>
#include <d3d11.h>
#include <D3DX11.h>
#include <iostream>
#include "Resource.h"

class GraphicsDX11 :
	public GraphicsInterface
{
private:
	D3D_DRIVER_TYPE				driverType;
	D3D_FEATURE_LEVEL			featureLevel;
	ID3D11Device				*device;
	
	IDXGISwapChain				*swapChain;

	ID3D11DeviceContext			*immediateContext;

	//renderTarget
	ID3D11Texture2D				*renderTargetTex;
	ID3D11RenderTargetView		*renderTargetView;
	ID3D11ShaderResourceView	*renderTargetResource;

	//depth stencil
	ID3D11Texture2D				*depthStencilTex;
	ID3D11DepthStencilView		*depthStencilView;
	ID3D11ShaderResourceView	*depthStencilResource;

	//viewports
	D3D11_VIEWPORT				viewPort;

	//blendStates
	ID3D11BlendState			*blendOption1;

	//depthStencilStates
	ID3D11DepthStencilState		*depthStencilStateEnable;
	ID3D11DepthStencilState		*depthStencilStateDisable;
	//rasterizerstates
	ID3D11RasterizerState		*rasterizerBackface;
	ID3D11RasterizerState		*rasterizerFrontface;
	//samplestates
	ID3D11SamplerState			*samplerLinear;


public:
	HRESULT						compileShaderFromFile( LPCSTR fileName, LPCSTR szEntryPoint, LPCSTR szShaderModel, ID3DBlob** ppBlobOut );

	GraphicsDX11();

	void init();
	void cleanUp();
	void compileShader();
	void presentSwapChain();

	~GraphicsDX11();
};

#endif // _WIN32