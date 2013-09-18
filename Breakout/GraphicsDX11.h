#ifdef _WIN32

#pragma once
#include <d3dcompiler.h>
#include <d3d11.h>
#include <D3DX11.h>
#include <iostream>
#include "Resource.h"
#include "TechniqueHLSL.h"

class GraphicsDX11
{
private:
	static GraphicsDX11			*instance;
	GraphicsDX11();

	//buffers
	ID3D11Buffer				*vBufferStatic;
	ID3D11Buffer				*instBuffer;		//instance buffer

	ID3D11Buffer				*vBufferDynamic;

	ID3D11Device				*device;
	D3D_DRIVER_TYPE				driverType;
	D3D_FEATURE_LEVEL			featureLevel;
	
	IDXGISwapChain				*swapChain;

	//techniques
	std::vector<TechniqueHLSL*>	techniques;

	//layouts
	ID3D11InputLayout			*simpleInputLayout;

	//renderTarget
	ID3D11Texture2D				*renderTargetTex;
	ID3D11RenderTargetView		*renderTargetView;
	ID3D11ShaderResourceView	*renderTargetResource;

	//depth stencil
	ID3D11Texture2D				*depthStencilTex;
	ID3D11DepthStencilView		*depthStencilView;
	ID3D11ShaderResourceView	*depthStencilResource;

	//constant buffer
	ID3D11Buffer				*cbWorld;
	ID3D11Buffer				*cbCameraMove;
	ID3D11Buffer				*cbOnce;

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
	/* is shader model 5 supported? */
	bool						shader5Support;
	/* obtain the one instance for this class */
	static GraphicsDX11			*getInstance()
	{
		if(!instance)
			instance = new GraphicsDX11();
		return instance;
	}
	/* initialization */
	void init(HWND *hWnd);
	void clearRenderTarget(float r, float g, float b);
	HRESULT compileShader( LPCSTR fileName, LPCSTR szEntryPoint, LPCSTR szShaderModel, ID3DBlob** ppBlobOut );
	void presentSwapChain();
	/* create constant buffer */
	bool createCBuffer(ID3D11Buffer **cb, UINT byteWidth, UINT registerIndex);
	/* create vertex buffer */
	bool createVBuffer( const D3D11_BUFFER_DESC *bd, const D3D11_SUBRESOURCE_DATA *initData, ID3D11Buffer **vBuffer );
	/* creates the static vertex buffer with all the static vertices. [immutable] */
	bool createVBufferStatic( std::vector<Vertex> vertices);
	/* creates the instance buffer for the static vertex buffer. [dynamic]*/
	bool createInstanceBuffer( std::vector<PerInstance> PerInstance);
	/* Feeds the instance buffer with instance data. (For the static vertex buffer) [dynamic] */
	void feedInstanceBuffer( std::vector<PerInstance> perInstance);
	/* get technique index by name. (returns -1 if none were found)*/
	int getTechIDByName(std::string name);
	/* set current technique for rendering */
	void useTechnique(unsigned int index);

	void draw(unsigned int startIndex, unsigned int vertexAmount);

	void useShaderResourceViews(ID3D11ShaderResourceView **views,int startSlot, int numberofViews);
	~GraphicsDX11();
};

#endif // _WIN32
