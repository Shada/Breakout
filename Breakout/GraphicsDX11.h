#ifdef _WIN32

#pragma once
#include <d3dcompiler.h>
#include <d3d11.h>
#include <D3DX11.h>
#include <iostream>
#include "TechniqueHLSL.h"
#include "LoadHandler.h"
#include "ObjectCore.h"

class GraphicsDX11
{
private:
	static GraphicsDX11			*instance;
	GraphicsDX11();

	Logic::ObjectCore			*objectCore;

	/*---------------------------------------------------------------
								buffers
	--------------------------------------------------------------*/
	/*the vertex buffer for static objects*/
	ID3D11Buffer				*vBufferStatic;
	/*the instance buffer for static objects (models and such)*/
	ID3D11Buffer				*instBuffer;
	/*the vertex buffer for dynamic objects*/
	ID3D11Buffer				*vBufferDynamic;
	/*the dynamic billboard buffer for menu UI*/
	ID3D11Buffer				*uiBufferDynamic;
	/*the dynamic buffer for text*/
	ID3D11Buffer				*textBufferDynamic;

	/*---------------------------------------------------------------
								textures
	--------------------------------------------------------------*/
	std::vector<ID3D11ShaderResourceView*> textures;


	ID3D11Device				*device;
	D3D_DRIVER_TYPE				driverType;
	D3D_FEATURE_LEVEL			featureLevel;
	
	IDXGISwapChain				*swapChain;

	/*---------------------------------------------------------------
								Techniques
	--------------------------------------------------------------*/
	std::vector<TechniqueHLSL*>	techniques;

	/*---------------------------------------------------------------
								Layouts
	--------------------------------------------------------------*/
	/*simple layout*/
	ID3D11InputLayout			*simpleInputLayout;
	ID3D11InputLayout			*uiLayout;
	ID3D11InputLayout			*fontLayout;

	/*---------------------------------------------------------------
								RenderTargets
	--------------------------------------------------------------*/
	ID3D11RenderTargetView		*renderTargetView;

	ID3D11Texture2D				*sceneTex;
	ID3D11RenderTargetView		*sceneRenderTargetView;
	ID3D11ShaderResourceView	*sceneShaderResource;

	ID3D11Texture2D				*reflTex;
	ID3D11RenderTargetView		*reflRenderTargetView;
	ID3D11ShaderResourceView	*reflShaderResource;

	//depth stencil
	ID3D11Texture2D				*depthStencilTex;
	ID3D11DepthStencilView		*depthStencilView;
	ID3D11ShaderResourceView	*depthStencilResource;

	//constant buffer
	ID3D11Buffer				*cbWorld;
	ID3D11Buffer				*cbCameraMove;
	ID3D11Buffer				*cbOnce;
	ID3D11Buffer				*cbFont;
	ID3D11Buffer				*cbWater;
	ID3D11Buffer				*cbWaterOnce;

	//viewports
	D3D11_VIEWPORT				viewPort;

	//blendStates
	ID3D11BlendState			*blendEnable;
	ID3D11BlendState			*blendDisable;

	//depthStencilStates
	ID3D11DepthStencilState		*depthStencilStateEnable;
	ID3D11DepthStencilState		*depthStencilStateDisable;
	//rasterizerstates
	ID3D11RasterizerState		*rasterizerBackface;
	ID3D11RasterizerState		*rasterizerFrontface;
	//samplestates
	ID3D11SamplerState			*samplerLinear;
	ID3D11SamplerState			*samplerSkybox;

	/*Gives you a pointer to a texture array*/
	void getTextureArray(std::vector<ID3D11ShaderResourceView*> *_textureArray);

	/* create vertex buffer */
	bool createVBuffer( const D3D11_BUFFER_DESC *bd, const D3D11_SUBRESOURCE_DATA *initData, ID3D11Buffer **vBuffer );

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
	void	setObjectCore(Logic::ObjectCore *objectCore) { this->objectCore = objectCore; }
	void	init(HWND *hWnd);
	void	clearRenderTarget(float r, float g, float b);
	HRESULT	compileShader( LPCSTR fileName, LPCSTR szEntryPoint, LPCSTR szShaderModel, ID3DBlob** ppBlobOut );
	void	presentSwapChain();
	/* create constant buffer */
	bool	createCBuffer(ID3D11Buffer **cb, UINT byteWidth, UINT registerIndex);
	
	void initVertexBuffer();

	/* creates the static vertex buffer with all the static vertices. [immutable] */
	bool	createVBufferStatic( std::vector<Vertex> vertices);
	/* creates the dynamic vertex buffer for menu items*/
	bool	createVBufferUI( unsigned int maxSize );
	/* creates the dynamic vertex buffer for text*/
	bool	createVBufferFont( unsigned int maxSize );
	/* creates the instance buffer for the static vertex buffer. [dynamic]*/
	bool	createInstanceBuffer( std::vector<PerInstance> PerInstance);
	/* Feeds the instance buffer with instance data. (For the static vertex buffer) [dynamic] */
	void	feedInstanceBuffer( std::vector<PerInstance> perInstance);
	/* get technique index by name. (returns -1 if none were found)*/
	int		getTechIDByName(const char *name);
	/* set current technique for rendering */
	void	useTechnique(unsigned int index);

	void	updateCBOnce(CBOnce cb)				{ immediateContext->UpdateSubresource(cbOnce, 0, NULL, &cb, 0, 0); }
	void	updateCBCameraMove(CBCameraMove cb) { immediateContext->UpdateSubresource(cbCameraMove, 0, NULL, &cb, 0, 0); }
	void	updateCBWorld(CBWorld cb)			{ immediateContext->UpdateSubresource(cbWorld, 0, NULL, &cb, 0, 0); }
	void	updateCBFont(CBFont cb)				{ immediateContext->UpdateSubresource(cbFont, 0, NULL, &cb, 0, 0); }
	void	updateCBWater(CBWater cb) 			{ immediateContext->UpdateSubresource(cbWater, 0, NULL, &cb, 0, 0); }
	void	updateCBWaterOnce(CBWaterOnce cb) 	{ immediateContext->UpdateSubresource(cbWaterOnce, 0, NULL, &cb, 0, 0); }

	void	draw();

	void	useShaderResourceViews(ID3D11ShaderResourceView **views,int startSlot, int numberofViews);
	~GraphicsDX11();
};

#endif // _WIN32
