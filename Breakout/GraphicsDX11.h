#pragma once
#include <d3dcompiler.h>
#include <d3d11.h>
#include <D3DX11.h>
#include <iostream>
#include "Resource.h"

class GraphicsDX11
{
private:
	static GraphicsDX11			*instance;
	GraphicsDX11();

	ID3D11Device				*device;
	D3D_DRIVER_TYPE				driverType;
	D3D_FEATURE_LEVEL			featureLevel;
	
	IDXGISwapChain				*swapChain;

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
	ID3D11BlendState			*blendEnable;

	//depthStencilStates
	ID3D11DepthStencilState		*depthStencilStateEnable;
	ID3D11DepthStencilState		*depthStencilStateDisable;
	//rasterizerstates
	ID3D11RasterizerState		*rasterizerBackface;
	ID3D11RasterizerState		*rasterizerFrontface;
	//samplestates
	ID3D11SamplerState			*samplerLinear;


public:
	ID3D11DeviceContext			*immediateContext;
	bool						shader5Support;

	static GraphicsDX11			*getInstance()
	{
		if(!instance)
			instance = new GraphicsDX11();
		return instance;
	}

	void init(HWND *hWnd);
	void clearRenderTarget(float r, float g, float b);
	HRESULT compileShader( LPCSTR fileName, LPCSTR szEntryPoint, LPCSTR szShaderModel, ID3DBlob** ppBlobOut );
	void presentSwapChain();


	~GraphicsDX11();
};

