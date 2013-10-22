#ifdef _WIN32

#include "GraphicsDX11.h"
#include <sstream>

GraphicsDX11	*GraphicsDX11::instance = NULL;

GraphicsDX11::GraphicsDX11()
{
	device						= NULL;
	immediateContext			= NULL;
	renderTargetView			= NULL;
	sceneTex					= NULL;
	sceneRenderTargetView		= NULL;
	sceneShaderResource			= NULL;
	reflTex						= NULL;
	reflRenderTargetView		= NULL;
	reflShaderResource			= NULL;
	depthStencilTex				= NULL;
	depthStencilView			= NULL;
	depthStencilResource		= NULL;
	blendEnable					= NULL;
	blendDisable				= NULL;
	depthStencilStateEnable		= NULL;
	depthStencilStateDisable	= NULL;
	rasterizerBackface			= NULL;
	rasterizerFrontface			= NULL;
	samplerLinear				= NULL;
	samplerSkybox				= NULL;
	shader5Support				= true;
	cbWater						= NULL;
	cbWaterOnce					= NULL;


	vBufferStatic				= NULL;
	vBufferDynamic				= NULL;
	uiBufferDynamic				= NULL;
	textBufferDynamic			= NULL;
	instBuffer					= NULL;
}

void GraphicsDX11::init(HWND *hWnd)
{
	HRESULT hr = S_OK;

	RECT	rc;
	GetClientRect( *hWnd, &rc );
	UINT width = SCRWIDTH;
	UINT height = SCRHEIGHT;

	UINT createDeviceFlags = 0;

	D3D_DRIVER_TYPE driverTypes[] =
    {
        D3D_DRIVER_TYPE_HARDWARE,
        D3D_DRIVER_TYPE_WARP,
        D3D_DRIVER_TYPE_REFERENCE,
    };
	UINT numDriverTypes = ARRAYSIZE( driverTypes );

    D3D_FEATURE_LEVEL featureLevels[] =
    {
        D3D_FEATURE_LEVEL_11_0,
        D3D_FEATURE_LEVEL_10_1,
        D3D_FEATURE_LEVEL_10_0,
    };

	UINT numFeatureLevels = ARRAYSIZE( featureLevels );

	DXGI_SWAP_CHAIN_DESC sd;
    ZeroMemory( &sd, sizeof( sd ) );
    sd.BufferCount = 1;
    sd.BufferDesc.Width = width;
    sd.BufferDesc.Height = height;
    sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    sd.BufferDesc.RefreshRate.Numerator = 100;
    sd.BufferDesc.RefreshRate.Denominator = 1;
    sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    sd.OutputWindow = (*hWnd);
    sd.SampleDesc.Count = 1;
    sd.SampleDesc.Quality = 0;
    sd.Windowed = TRUE;

	for( UINT driverTypeIndex = 0; driverTypeIndex < numDriverTypes; driverTypeIndex++ )
    {
		driverType = driverTypes[driverTypeIndex];
        hr = D3D11CreateDeviceAndSwapChain( NULL, driverType, NULL, createDeviceFlags, featureLevels, numFeatureLevels,
                                            D3D11_SDK_VERSION, &sd, &swapChain, &device, &featureLevel, &immediateContext );
        if( SUCCEEDED( hr ) )
            break;
    }
    if( FAILED( hr ) )
	{
		MessageBox( NULL, "Error creating device and swapchain","GraphicsDX11 Error", S_OK);
        return;
	}

	// Create a render target view
    ID3D11Texture2D* backBuffer = NULL;
    hr = swapChain->GetBuffer( 0, __uuidof( ID3D11Texture2D ), ( LPVOID* )&backBuffer );
    if( FAILED( hr ) )
	{
		MessageBox( NULL, "Error creating backbuffer","GraphicsDX11 Error", S_OK);
        return;
	}

	hr = device->CreateRenderTargetView( backBuffer, NULL, &renderTargetView );
    backBuffer->Release();
    if( FAILED( hr ) )
	{
		MessageBox( NULL, "Error creating renderTargetView","GraphicsDX11 Error", S_OK);
        return;
	}

	//create depth stencil texture
	D3D11_TEXTURE2D_DESC descDepth;
    ZeroMemory( &descDepth, sizeof(descDepth) );
    descDepth.Width					= width;
    descDepth.Height				= height;
    descDepth.MipLevels				= 1;
    descDepth.ArraySize				= 1;
	descDepth.Format				= DXGI_FORMAT_R32_TYPELESS;
    descDepth.SampleDesc.Count		= 1;
    descDepth.SampleDesc.Quality	= 0;
    descDepth.Usage					= D3D11_USAGE_DEFAULT;
	descDepth.BindFlags				= D3D11_BIND_DEPTH_STENCIL | D3D11_BIND_SHADER_RESOURCE; //also a shader resource
    descDepth.CPUAccessFlags		= 0;
    descDepth.MiscFlags				= 0;
    hr								= device->CreateTexture2D( &descDepth, NULL, &depthStencilTex );
    if( FAILED( hr ) )
	{
		MessageBox( NULL, "Error creating depth stencil texture","GraphicsDX11 Error", S_OK);
        return;
	}

	// Create the depth stencil view
    D3D11_DEPTH_STENCIL_VIEW_DESC descDSV;
    ZeroMemory( &descDSV, sizeof(descDSV) );
    descDSV.Format					= DXGI_FORMAT_D32_FLOAT;
    descDSV.ViewDimension			= D3D11_DSV_DIMENSION_TEXTURE2D;
    descDSV.Texture2D.MipSlice		= 0;
    hr								= device->CreateDepthStencilView( depthStencilTex, &descDSV, &depthStencilView );
    if( FAILED( hr ) )
	{
		MessageBox( NULL, "Error creating depth stencil view","GraphicsDX11 Error", S_OK);
        return;
	}

	//create the depth stencil as a shader resource
	D3D11_SHADER_RESOURCE_VIEW_DESC	descSRV;
    descSRV.Format						= DXGI_FORMAT_R32_FLOAT;	// Change to R32_FLOAT from typeless
    descSRV.ViewDimension				= D3D10_SRV_DIMENSION_TEXTURE2D;
    descSRV.Texture2D.MostDetailedMip	= 0;
	descSRV.Texture2D.MipLevels			= 1;
    hr									= device->CreateShaderResourceView( depthStencilTex,
																			&descSRV,
																			&depthStencilResource );
	if( FAILED(hr) )
	{
		MessageBox(NULL, "Error creating depth stencil shader resource!", "GraphicsDX11 Error", S_OK);
		return;
	}

	//--- additional rendertargets and shader resources for water post process effect ---//
	D3D11_TEXTURE2D_DESC dsTex;
	ZeroMemory(&dsTex, sizeof(dsTex));
	dsTex.Width					= SCRWIDTH;
	dsTex.Height				= SCRHEIGHT;
	dsTex.MipLevels				= 1;
	dsTex.ArraySize				= 1;
	dsTex.SampleDesc.Count		= 1;
	dsTex.SampleDesc.Quality	= 0;
	dsTex.Format				= DXGI_FORMAT_R16G16B16A16_UNORM;
	dsTex.Usage					= D3D11_USAGE_DEFAULT;
	dsTex.BindFlags				= D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
	dsTex.CPUAccessFlags		= 0;
	hr = device->CreateTexture2D( &dsTex, NULL, &sceneTex );
	if(FAILED(hr))
	{
		MessageBox( NULL, "Failed to create scene render target texture for scene","GraphicsCore Error",MB_OK);
		return;
	}
	// this texture could be lower res if needed
	hr = device->CreateTexture2D( &dsTex, NULL, &reflTex );
	if(FAILED(hr))
	{
		MessageBox( NULL, "Failed to create scene render target texture for reflections","GraphicsCore Error",MB_OK);
		return;
	}
	D3D11_RENDER_TARGET_VIEW_DESC rtvDesc;
	rtvDesc.Format				= dsTex.Format;
	rtvDesc.ViewDimension		= D3D11_RTV_DIMENSION_TEXTURE2D;
	rtvDesc.Texture2D.MipSlice	= 0;
	hr = device->CreateRenderTargetView(sceneTex,&rtvDesc, &sceneRenderTargetView);
	if(FAILED(hr))
	{
		MessageBox( NULL, "Failed to create scene render target view for scene","GraphicsCore Error",MB_OK);
		return;
	}
	hr = device->CreateRenderTargetView(reflTex,&rtvDesc, &reflRenderTargetView);
	if(FAILED(hr))
	{
		MessageBox( NULL, "Failed to create scene render target view for reflections","GraphicsCore Error",MB_OK);
		return;
	}

	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
	ZeroMemory( &srvDesc, sizeof(srvDesc));
	srvDesc.Format						= dsTex.Format;
	srvDesc.ViewDimension				= D3D11_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MipLevels			= 1;
	srvDesc.Texture2D.MostDetailedMip	= 0;
	hr = device->CreateShaderResourceView(sceneTex, &srvDesc, &sceneShaderResource);
	if(FAILED(hr))
	{
		MessageBox( NULL, "Failed to create scene shader resource view for scene","GraphicsCore Error",MB_OK);
		return;
	}
	hr = device->CreateShaderResourceView(reflTex, &srvDesc, &reflShaderResource);
	if(FAILED(hr))
	{
		MessageBox( NULL, "Failed to create scene shader resource view for reflections","GraphicsCore Error",MB_OK);
		return;
	}

	// Setup the viewport
    viewPort.Width		= (FLOAT)SCRWIDTH;
    viewPort.Height		= (FLOAT)SCRHEIGHT;
    viewPort.MinDepth		= 0.0f;
    viewPort.MaxDepth		= 1.0f;
    viewPort.TopLeftX		= 0;
    viewPort.TopLeftY		= 0;
	immediateContext->RSSetViewports( 1, &viewPort );

	techniques = std::vector<TechniqueHLSL*>();
	
	techniques.push_back( new TechniqueHLSL(device, "techSkybox", "shaders/hlsl/vsSkybox.fx", "vs_skybox","","","shaders/hlsl/psSkybox.fx","ps_skybox") );
	techniques.push_back( new TechniqueHLSL(device, "techSimple",	"shaders/hlsl/vsSimple.fx",	"vs",	"",							"",		"shaders/hlsl/psSimple.fx",	"ps") );
	techniques.push_back( new TechniqueHLSL(device, "techUI",		"shaders/hlsl/vsBBUI.fx",	"vs",	"shaders/hlsl/gsBBUI.fx",	"gs",	"shaders/hlsl/psBBUI.fx",	"ps") );
	techniques.push_back( new TechniqueHLSL(device, "techFont",		"shaders/hlsl/vsFont.fx",	"vs",	"shaders/hlsl/gsFont.fx",	"gs",	"shaders/hlsl/psFont.fx",	"ps") );
	techniques.push_back( new TechniqueHLSL(device, "techSkyboxRefl", "shaders/hlsl/vsSkyboxRefl.fx", "vs_skybox","","","shaders/hlsl/psSkybox.fx","ps_skybox") );
	techniques.push_back( new TechniqueHLSL(device, "techQuad", "shaders/hlsl/vsQuad.fx", "vs_quad","shaders/hlsl/gsQuad.fx","gs_quad","shaders/hlsl/psQuad.fx","ps_quad") );
	techniques.push_back( new TechniqueHLSL(device, "techRefl", "shaders/hlsl/vsRefl.fx", "vs_refl","","","shaders/hlsl/psRefl.fx","ps_refl") );
	techniques.push_back( new TechniqueHLSL(device, "techWater", "shaders/hlsl/vsQuad.fx", "vs_quad","shaders/hlsl/gsQuad.fx","gs_quad","shaders/hlsl/psWater.fx","ps_water") );

	D3D11_INPUT_ELEMENT_DESC simpleLayoutDesc[] = 
	{
		{ "POSITION",	0, DXGI_FORMAT_R32G32B32_FLOAT,		0, 0,					D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL",		0, DXGI_FORMAT_R32G32B32_FLOAT,		0, sizeof(float) * 3,	D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD",	0, DXGI_FORMAT_R32G32_FLOAT,		0, sizeof(float) * 6,	D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};
	UINT techIndex = getTechIDByName("techSimple");
	hr = device->CreateInputLayout( simpleLayoutDesc, ARRAYSIZE(simpleLayoutDesc), techniques.at(techIndex)->getInputSignature(),
									techniques.at(techIndex)->getInputSignatureSize(), &simpleInputLayout );
	if(FAILED(hr))
	{
		MessageBox( NULL, "Failed to create simpleInputLayout","GraphicsDX11 Error",MB_OK);
		return;
	}
	D3D11_INPUT_ELEMENT_DESC uiLayoutDesc[] =
	{
		{ "POSITION",	0, DXGI_FORMAT_R32G32_FLOAT,		0,	0,					D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "SIZE",		0, DXGI_FORMAT_R32G32_FLOAT,		0,	sizeof(float) * 2,	D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "ROTATION",	0, DXGI_FORMAT_R32_FLOAT,			0,	sizeof(float) * 4,	D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TINTALPHA",	0, DXGI_FORMAT_R32G32B32A32_FLOAT,	0,	sizeof(float) * 5,	D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXINDEX",	0, DXGI_FORMAT_R32_UINT,			0,	sizeof(float) * 9,	D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};
	techIndex = getTechIDByName("techUI");
	hr = device->CreateInputLayout(uiLayoutDesc, ARRAYSIZE(uiLayoutDesc), techniques.at(techIndex)->getInputSignature(),
									techniques.at(techIndex)->getInputSignatureSize(), &uiLayout );
	if(FAILED(hr))
	{
		MessageBox( NULL, "Failed to create uiLayout","GraphicsDX11 Error",MB_OK);
		return;
	}

	D3D11_INPUT_ELEMENT_DESC fontLayoutDesc[] =
	{
		{ "POSITION",	0, DXGI_FORMAT_R32_FLOAT,		0,	0,					D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD",	0, DXGI_FORMAT_R32G32B32A32_FLOAT,	0,	sizeof(float),	D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};
	techIndex = getTechIDByName("techFont");
	hr = device->CreateInputLayout(fontLayoutDesc, ARRAYSIZE(fontLayoutDesc), techniques.at(techIndex)->getInputSignature(),
									techniques.at(techIndex)->getInputSignatureSize(), &fontLayout );
	if(FAILED(hr))
	{
		MessageBox( NULL, "Failed to create fontLayout","GraphicsDX11 Error",MB_OK);
		return;
	}

	/*D3D11_INPUT_ELEMENT_DESC simpleLayoutInst[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, sizeof(float) * 3, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, sizeof(float) * 6, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "PERINST", 0, DXGI_FORMAT_R32G32B32_FLOAT, 1, 0, D3D11_INPUT_PER_INSTANCE_DATA, 1 },
		{ "PERINST", 1, DXGI_FORMAT_R32_FLOAT, 1, sizeof(float) * 3, D3D11_INPUT_PER_INSTANCE_DATA, 1 }
	};
	hr = device->CreateInputLayout(simpleLayoutInst, ARRAYSIZE(simpleLayoutInst), techInstModel->getInputSignature(),techInstModel->getInputSignatureSize(), &instanceLayout);
	if(FAILED(hr))
	{
		MessageBox( NULL, "Failed to create instancedInputLayout","GraphicsDX11 Error",MB_OK);
		return;
	}*/

	if( !createCBuffer( &cbWorld, sizeof(CBWorld), 0 ) )
		return;
	if( !createCBuffer( &cbCameraMove, sizeof(CBCameraMove), 1 ) )
		return;
	if( !createCBuffer( &cbOnce, sizeof(CBOnce), 2 ) )
		return;
	if( !createCBuffer( &cbFont, sizeof(CBFont), 3 ) )
		return;
	if( !createCBuffer(&cbWater, sizeof(CBOnce), 4))
		return;
	if( !createCBuffer(&cbWaterOnce, sizeof(CBOnce), 5))
		return;

	//create samplerstates
	D3D11_SAMPLER_DESC sampDesc;
	ZeroMemory( &sampDesc, sizeof(sampDesc));
	sampDesc.Filter			= D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	sampDesc.AddressU		= D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.AddressV		= D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.AddressW		= D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.ComparisonFunc	= D3D11_COMPARISON_NEVER;
	sampDesc.MinLOD			= -D3D11_FLOAT32_MAX;
	sampDesc.MaxLOD			= D3D11_FLOAT32_MAX;

	hr = device->CreateSamplerState( &sampDesc, &samplerLinear);
	if(FAILED(hr))
		return;

	sampDesc.AddressU		= D3D11_TEXTURE_ADDRESS_CLAMP;
	sampDesc.AddressV		= D3D11_TEXTURE_ADDRESS_CLAMP;
	sampDesc.AddressW		= D3D11_TEXTURE_ADDRESS_CLAMP;
	hr = device->CreateSamplerState( &sampDesc, &samplerSkybox);
	if(FAILED(hr))
		return;

	//create blendstates
	D3D11_BLEND_DESC blendDesc;
	ZeroMemory(&blendDesc,sizeof(blendDesc));
	/*blendDesc.AlphaToCoverageEnable						= FALSE;
	blendDesc.IndependentBlendEnable					= FALSE;*/
	blendDesc.RenderTarget[0].BlendEnable				= TRUE;
	blendDesc.RenderTarget[0].SrcBlend					= D3D11_BLEND_SRC_ALPHA;
	blendDesc.RenderTarget[0].DestBlend					= D3D11_BLEND_INV_SRC_ALPHA;
    blendDesc.RenderTarget[0].BlendOp					= D3D11_BLEND_OP_ADD;
    blendDesc.RenderTarget[0].SrcBlendAlpha				= D3D11_BLEND_ONE;
    blendDesc.RenderTarget[0].DestBlendAlpha			= D3D11_BLEND_ZERO;
    blendDesc.RenderTarget[0].BlendOpAlpha				= D3D11_BLEND_OP_ADD;
    blendDesc.RenderTarget[0].RenderTargetWriteMask		= 0x0f;

	hr = device->CreateBlendState(&blendDesc,&blendEnable);
	if(FAILED(hr))
		return;

	blendDesc.RenderTarget[0].BlendEnable				= FALSE;

	hr = device->CreateBlendState(&blendDesc,&blendDisable);
	if(FAILED(hr))
		return;
	

	//create depthstencil states
	D3D11_DEPTH_STENCIL_DESC depthDesc;
	depthDesc.DepthEnable					= TRUE;
	depthDesc.DepthWriteMask				= D3D11_DEPTH_WRITE_MASK_ALL;
	depthDesc.DepthFunc						= D3D11_COMPARISON_LESS;
	depthDesc.StencilEnable					= FALSE;
	depthDesc.StencilReadMask				= D3D11_DEFAULT_STENCIL_READ_MASK;
	depthDesc.StencilWriteMask				= D3D11_DEFAULT_STENCIL_WRITE_MASK;
	depthDesc.FrontFace.StencilFunc			= D3D11_COMPARISON_ALWAYS;
	depthDesc.BackFace.StencilFunc			= D3D11_COMPARISON_ALWAYS;
	depthDesc.FrontFace.StencilDepthFailOp	= D3D11_STENCIL_OP_KEEP;
	depthDesc.BackFace.StencilDepthFailOp	= D3D11_STENCIL_OP_KEEP;
	depthDesc.FrontFace.StencilPassOp		= D3D11_STENCIL_OP_KEEP;
	depthDesc.BackFace.StencilPassOp		= D3D11_STENCIL_OP_KEEP;
	depthDesc.FrontFace.StencilFailOp		= D3D11_STENCIL_OP_KEEP;
	depthDesc.BackFace.StencilFailOp		= D3D11_STENCIL_OP_KEEP;

	hr = device->CreateDepthStencilState(&depthDesc, &depthStencilStateEnable);
	if(FAILED(hr))
		return;

	depthDesc.DepthEnable					= FALSE;
	hr = device->CreateDepthStencilState(&depthDesc, &depthStencilStateDisable);
	if(FAILED(hr))
		return;

	//create rasterizerstates
	D3D11_RASTERIZER_DESC rasterDesc;
	ZeroMemory( &rasterDesc,sizeof(rasterDesc));
	rasterDesc.FillMode					= D3D11_FILL_SOLID;
	rasterDesc.CullMode					= D3D11_CULL_BACK;
    rasterDesc.FrontCounterClockwise	= false;
    rasterDesc.DepthBias				= 0;
    rasterDesc.DepthBiasClamp			= 0;
    rasterDesc.SlopeScaledDepthBias		= 0;
    rasterDesc.DepthClipEnable			= true;
    rasterDesc.ScissorEnable			= false;
    rasterDesc.MultisampleEnable		= false;
    rasterDesc.AntialiasedLineEnable	= false;

	hr = device->CreateRasterizerState( &rasterDesc, &rasterizerBackface );
	if(FAILED(hr))
		return;
	rasterDesc.CullMode = D3D11_CULL_FRONT;
	hr = device->CreateRasterizerState( &rasterDesc, &rasterizerFrontface );
	if(FAILED(hr))
		return;

	initVertexBuffer();
	createVBufferUI(100);
	createVBufferFont(100);
	getTextureArray(&textures);
}

HRESULT GraphicsDX11::compileShader( LPCSTR fileName, LPCSTR szEntryPoint, LPCSTR szShaderModel, ID3DBlob** ppBlobOut )
{
	 HRESULT hr = S_OK;

    DWORD dwShaderFlags = D3DCOMPILE_ENABLE_STRICTNESS;
#if defined( DEBUG ) || defined( _DEBUG )
    // Set the D3DCOMPILE_DEBUG flag to embed debug information in the shaders.
    // Setting this flag improves the shader debugging experience, but still allows 
    // the shaders to be optimized and to run exactly the way they will run in 
    // the release configuration of this program.
    dwShaderFlags |= D3DCOMPILE_DEBUG;
#endif

    ID3DBlob* pErrorBlob;
    hr = D3DX11CompileFromFile( fileName , NULL, NULL, szEntryPoint, szShaderModel, 
        dwShaderFlags, 0, NULL, ppBlobOut, &pErrorBlob, NULL );
    if( FAILED(hr) )
    {
        if( pErrorBlob != NULL )
            OutputDebugStringA( (char*)pErrorBlob->GetBufferPointer() );
        if( pErrorBlob ) pErrorBlob->Release();
        return hr;
    }
    if( pErrorBlob ) pErrorBlob->Release();

    return S_OK;
}

void GraphicsDX11::clearRenderTarget(float r, float g, float b)
{
	float clearColor[4] = {r, g, b, 1};
	immediateContext->ClearRenderTargetView(renderTargetView, clearColor);
	immediateContext->ClearRenderTargetView(sceneRenderTargetView, clearColor);
	immediateContext->ClearRenderTargetView(reflRenderTargetView, clearColor);
	immediateContext->ClearDepthStencilView(depthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);
}

void GraphicsDX11::presentSwapChain()
{
	swapChain->Present( 0, 0 );
}
bool GraphicsDX11::createCBuffer(ID3D11Buffer **cb, UINT byteWidth, UINT registerIndex)
{
	HRESULT hr;
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage			= D3D11_USAGE_DEFAULT;
	bd.BindFlags		= D3D11_BIND_CONSTANT_BUFFER;
	bd.CPUAccessFlags	= 0;
	bd.ByteWidth		= byteWidth;

	hr = device->CreateBuffer( &bd,NULL,cb);
	if(FAILED(hr))
	{
		std::stringstream numb;
		numb << registerIndex;
		std::string message = "Failed to create constant buffer " + numb.str();
		MessageBox( NULL, message.c_str(),"GraphicsCore Error",MB_OK);
		return false;
	}
	immediateContext->VSSetConstantBuffers(registerIndex, 1, cb);
	//immediateContext->DSSetConstantBuffers(registerIndex, 1, cb);
	immediateContext->GSSetConstantBuffers(registerIndex, 1, cb);
	immediateContext->PSSetConstantBuffers(registerIndex, 1, cb);
	//immediateContext->HSSetConstantBuffers(registerIndex, 1, cb);
	return true;
}

void GraphicsDX11::initVertexBuffer()
{
	Resources::LoadHandler *loader = Resources::LoadHandler::getInstance();
	std::vector<Vertex> vertices;
	int start = vertices.size();
	for(unsigned int i = 0; i < loader->getModelSize(); i++)
	{
		loader->getModel(i)->setStartIndex(start);
		vertices.insert(vertices.end(), loader->getModel(i)->getData()->begin(), loader->getModel(i)->getData()->end());
		start += loader->getModel(i)->getData()->size();
	}
	createVBufferStatic(vertices);
}

bool GraphicsDX11::createVBufferStatic( std::vector<Vertex>	vertices )
{
	D3D11_BUFFER_DESC bd;
	ZeroMemory( &bd, sizeof(bd) );
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof( Vertex ) * vertices.size();
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = 0;
	D3D11_SUBRESOURCE_DATA initData;
	ZeroMemory( &initData, sizeof( initData ) );
	initData.pSysMem = &vertices[0];
	if(! createVBuffer(&bd, &initData, &vBufferStatic) )
		return false;
	return true;
}



bool GraphicsDX11::createInstanceBuffer( std::vector<PerInstance> perInstance )
{
	D3D11_BUFFER_DESC bd;
	ZeroMemory( &bd, sizeof(bd) );
	bd.Usage = D3D11_USAGE_DYNAMIC;
	bd.ByteWidth = sizeof( PerInstance ) * perInstance.size();
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	D3D11_SUBRESOURCE_DATA initData;
	ZeroMemory( &initData, sizeof( initData ) );
	initData.pSysMem = &perInstance[0];
	if(! createVBuffer(&bd, &initData, &instBuffer) )
		return false;
	return true;
}

bool GraphicsDX11::createVBufferUI( unsigned int maxSize )
{
	D3D11_BUFFER_DESC bd;
	ZeroMemory( &bd, sizeof(bd) );
	bd.Usage = D3D11_USAGE_DYNAMIC;
	bd.ByteWidth = sizeof( BBUI ) * maxSize;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	if(! createVBuffer(&bd, NULL, &uiBufferDynamic) )
		return false;
	return true;
}

bool GraphicsDX11::createVBufferFont( unsigned int maxSize )
{
	D3D11_BUFFER_DESC bd;
	ZeroMemory( &bd, sizeof(bd) );
	bd.Usage = D3D11_USAGE_DYNAMIC;
	bd.ByteWidth = sizeof( BBFont ) * maxSize;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	if(! createVBuffer(&bd, NULL, &textBufferDynamic) )
		return false;
	return true;
}

bool GraphicsDX11::createVBuffer( const D3D11_BUFFER_DESC *bd, const D3D11_SUBRESOURCE_DATA *initData, ID3D11Buffer **vBuffer )
{
	HRESULT hr = device->CreateBuffer( bd, initData, vBuffer );
	if( FAILED( hr ) )
	{
		MessageBox(NULL,"Failed to create vertex buffer!", "GraphicsDX11 Error", S_OK);
		return false;
	}
	return true;
}
/*
techSimple
techUI
*/
int GraphicsDX11::getTechIDByName( const char *name )
{
	for(unsigned int i = 0; i < techniques.size(); i++)
		if(techniques.at(i)->getName() == name)
			return i;
	return -1;
}

void GraphicsDX11::useTechnique( unsigned int id )
{
	techniques.at(id)->useTechnique();
}

void GraphicsDX11::draw()
{
	switch(Global::getInstance()->gameState)
	{
	case GameState::GAME_MENU:
		{
			drawMenu();
			break;
		}
	case GameState::GAME_PLAY:
		{
			drawGame();
			break;
		}
	}
}

void GraphicsDX11::drawGame()
{
	Resources::LoadHandler *lh = Resources::LoadHandler::getInstance();
	CBWorld cbWorld;

	ID3D11ShaderResourceView *const nullSRV[5] = {NULL,NULL,NULL,NULL,NULL};

	unsigned int vertexAmount, startIndex, modelID;

	float blendFactor[4];
	blendFactor[0] = 0.0f;
	blendFactor[1] = 0.0f;
	blendFactor[2] = 0.0f;
	blendFactor[3] = 0.0f;

	UINT stride = sizeof(Vertex);
	UINT offset = 0;

	//om
	immediateContext->OMSetBlendState(blendDisable, blendFactor, 0xffffffff);
	immediateContext->PSSetShaderResources(0,1,&nullSRV[0]);
	immediateContext->OMSetRenderTargets(1, &reflRenderTargetView, depthStencilView);

	//ia
	immediateContext->IASetVertexBuffers(0, 1, &vBufferStatic, &stride, &offset);
	immediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	immediateContext->IASetInputLayout(simpleInputLayout);

	//rs
	immediateContext->RSSetViewports(1, &viewPort);
	
	immediateContext->IASetInputLayout(simpleInputLayout);


	//--------------------------------------------------------------------------------
	//                                 reflections
	//-------------------------------------------------------------------------------
	//       draw everything again, but using reflecion techniques

	//--------------------------------------------------------------------------------
	//                                     skybox
	//-------------------------------------------------------------------------------
	immediateContext->PSSetSamplers(1, 1, &samplerSkybox);
	techniques.at( getTechIDByName( "techSkyboxRefl" ) )->useTechnique();
	immediateContext->PSSetShaderResources(0,1,&textures.at(objectCore->skybox->getTextureID()));
	modelID					= objectCore->skybox->getModelID();
	vertexAmount		= lh->getModel( modelID )->getVertexAmount();
	startIndex			= lh->getModel( modelID )->getStartIndex();

	immediateContext->OMSetDepthStencilState(depthStencilStateDisable,0);
	immediateContext->RSSetState(rasterizerFrontface);
	immediateContext->Draw(vertexAmount, startIndex);

	//--------------------------------------------------------------------------------------

	immediateContext->OMSetDepthStencilState(depthStencilStateEnable, 0);
	immediateContext->RSSetState(rasterizerBackface);

	techniques.at( getTechIDByName( "techRefl" ) )->useTechnique();
	
	//--------------------------------------------------------------------------------
	//                                     Pad
	//--------------------------------------------------------------------------------

	cbWorld.world		= objectCore->pad->getWorld();
	cbWorld.worldInv	= objectCore->pad->getWorldInv();
	updateCBWorld(cbWorld);


	modelID			= objectCore->pad->getModelID();
	vertexAmount	= lh->getModel( modelID )->getVertexAmount();
	startIndex		= lh->getModel( modelID )->getStartIndex();

	immediateContext->PSSetShaderResources(0,1,&textures.at(objectCore->pad->getTextureID()));
	immediateContext->Draw(vertexAmount, startIndex);

	//--------------------------------------------------------------------------------
	//                                     bricks
	//--------------------------------------------------------------------------------

	for(unsigned int i = 0; i < objectCore->bricks.size(); i++)
	{
		cbWorld.world		= objectCore->bricks.at(i)->getWorld();
		cbWorld.worldInv	= objectCore->bricks.at(i)->getWorldInv();
		updateCBWorld(cbWorld);

		immediateContext->PSSetShaderResources(0,1,&textures.at(objectCore->bricks.at(i)->getTextureID()));
		modelID				= objectCore->bricks.at(i)->getModelID();
		vertexAmount		= lh->getModel( modelID )->getVertexAmount();
		startIndex			= lh->getModel( modelID )->getStartIndex();

		immediateContext->Draw(vertexAmount, startIndex);
	}

	// clear depth stencil so that the normal drawing  wont get disturbed and have issues with an already
	// filled depth stencil
	immediateContext->ClearDepthStencilView( depthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0 );
	//------------- normal draw ---------------//
	immediateContext->OMSetRenderTargets(1, &sceneRenderTargetView, depthStencilView);

	//--------------------------------------------------------------------------------
	//                                     skybox
	//-------------------------------------------------------------------------------
	immediateContext->PSSetSamplers(0, 1, &samplerSkybox);
	techniques.at( getTechIDByName( "techSkybox" ) )->useTechnique();
	immediateContext->PSSetShaderResources(0,1,&textures.at(objectCore->skybox->getTextureID()));
	modelID					= objectCore->skybox->getModelID();
	vertexAmount		= lh->getModel( modelID )->getVertexAmount();
	startIndex			= lh->getModel( modelID )->getStartIndex();

	immediateContext->OMSetDepthStencilState(depthStencilStateDisable, 0);
	immediateContext->RSSetState(rasterizerFrontface);
	immediateContext->Draw(vertexAmount, startIndex);

	//--------------------------------------------------------------------------------------
	 
	immediateContext->OMSetDepthStencilState(depthStencilStateEnable, 0);
	immediateContext->RSSetState(rasterizerBackface);

	techniques.at( getTechIDByName( "techSimple" ) )->useTechnique();
	//--------------------------------------------------------------------------------
	//                                    Ball(s)
	//--------------------------------------------------------------------------------
	for(unsigned int i = 0; i < objectCore->ball.size(); i++)
	{
		cbWorld.world		= objectCore->ball.at(i)->getWorld();
		cbWorld.worldInv	= objectCore->ball.at(i)->getWorldInv();
		updateCBWorld(cbWorld);

		immediateContext->PSSetSamplers(0, 1, &samplerLinear);
		immediateContext->IASetInputLayout(simpleInputLayout);

		modelID			= objectCore->ball.at(i)->getModelID();
		vertexAmount	= lh->getModel( modelID )->getVertexAmount();
		startIndex		= lh->getModel( modelID )->getStartIndex();

		immediateContext->PSSetShaderResources(0,1,&textures.at(objectCore->ball.at(i)->getTextureID()));
		immediateContext->Draw(vertexAmount, startIndex);
	}

	//--------------------------------------------------------------------------------
	//                                     Pad
	//--------------------------------------------------------------------------------

	cbWorld.world		= objectCore->pad->getWorld();
	cbWorld.worldInv	= objectCore->pad->getWorldInv();
	updateCBWorld(cbWorld);


	modelID			= objectCore->pad->getModelID();
	vertexAmount	= lh->getModel( modelID )->getVertexAmount();
	startIndex		= lh->getModel( modelID )->getStartIndex();

	immediateContext->PSSetShaderResources(0,1,&textures.at(objectCore->pad->getTextureID()));
	immediateContext->Draw(vertexAmount, startIndex);

	//--------------------------------------------------------------------------------
	//                                     bricks
	//--------------------------------------------------------------------------------

	for(unsigned int i = 0; i < objectCore->bricks.size(); i++)
	{
		cbWorld.world		= objectCore->bricks.at(i)->getWorld();
		cbWorld.worldInv	= objectCore->bricks.at(i)->getWorldInv();
		updateCBWorld(cbWorld);

		immediateContext->PSSetShaderResources(0,1,&textures.at(objectCore->bricks.at(i)->getTextureID()));
		modelID				= objectCore->bricks.at(i)->getModelID();
		vertexAmount		= lh->getModel( modelID )->getVertexAmount();
		startIndex			= lh->getModel( modelID )->getStartIndex();

		immediateContext->Draw(vertexAmount, startIndex);
	}

	// -------------------------------------------------------------------------------
	//                                  water
	//--------------------------------------------------------------------------------
	immediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_POINTLIST);
	immediateContext->OMSetRenderTargets(1, &renderTargetView, NULL);
	immediateContext->PSSetShaderResources(0,1,&sceneShaderResource);
	immediateContext->PSSetShaderResources(1,1,&depthStencilResource);
	immediateContext->PSSetShaderResources(2,1,&reflShaderResource);
	immediateContext->PSSetShaderResources(3,5,&textures[36]);
	techniques.at(getTechIDByName("techWater"))->useTechnique();
	immediateContext->Draw(1,0);

	immediateContext->PSSetShaderResources(0,5,&nullSRV[0]);
	
	//--------------------------------------------------------------------------------
	//                                     UI
	//--------------------------------------------------------------------------------

	stride = sizeof( BBUI );
	offset = 0;

	immediateContext->RSSetState(rasterizerBackface);

	immediateContext->OMSetDepthStencilState(depthStencilStateDisable, 0);
	
	D3D11_MAPPED_SUBRESOURCE updateData;
	ZeroMemory( &updateData, sizeof( updateData ) );

	immediateContext->Map(uiBufferDynamic, 0, D3D11_MAP_WRITE_DISCARD, 0, &updateData);
	memcpy( updateData.pData, &objectCore->uiBillboards[0], sizeof(BBUI)* objectCore->uiBillboards.size() );
	immediateContext->Unmap(uiBufferDynamic, 0);

	immediateContext->OMSetBlendState(blendEnable, blendFactor, 0xffffffff);
	immediateContext->IASetVertexBuffers( 0, 1, &uiBufferDynamic, &stride, &offset );
	immediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_POINTLIST);
	techniques.at( getTechIDByName( "techUI" ) )->useTechnique();
	immediateContext->IASetInputLayout(uiLayout);

	cbWorld.world			= Matrix(	1,0,0,0,
									0,1,0,0,
									0,0,1,0,
									0,0,0,1	);

	cbWorld.worldInv		= Matrix(	1,0,0,0,
									0,1,0,0,
									0,0,1,0,
									0,0,0,1	);
	updateCBWorld( cbWorld );

	immediateContext->PSSetShaderResources( 0,1,&textures.at( objectCore->SideBar->getTexIndex() ) );
	vertexAmount	= objectCore->uiBillboards.size();
	startIndex		= 0;

	immediateContext->Draw( vertexAmount, startIndex );

	//--------------------------------------------------------------------------------
	//                                     Text
	//--------------------------------------------------------------------------------
	stride = sizeof( BBFont );
	offset = 0;

	immediateContext->RSSetState(rasterizerBackface);

	D3D11_MAPPED_SUBRESOURCE textData;
	ZeroMemory( &textData, sizeof( textData ) );

	immediateContext->Map(textBufferDynamic, 0, D3D11_MAP_WRITE_DISCARD, 0, &textData);
	memcpy( textData.pData, &objectCore->fontBillboards[0], sizeof(BBFont)* objectCore->fontBillboards.size() );
	immediateContext->Unmap(textBufferDynamic, 0);

	immediateContext->IASetVertexBuffers( 0, 1, &textBufferDynamic, &stride, &offset );
	immediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_POINTLIST);
	techniques.at( getTechIDByName( "techFont" ) )->useTechnique();
	immediateContext->IASetInputLayout(fontLayout);
	immediateContext->PSSetShaderResources( 0,1,&textures.at( 4 ) );

	vertexAmount		= objectCore->testText->getTextSize();
	startIndex			= objectCore->testText->getVBStartIndex();

	objectCore->testText->updateCB();

	immediateContext->Draw( vertexAmount, startIndex );
}
void GraphicsDX11::drawMenu()
{
	Resources::LoadHandler *lh = Resources::LoadHandler::getInstance();
	CBWorld cbWorld;

	ID3D11ShaderResourceView *const nullSRV[5] = {NULL,NULL,NULL,NULL,NULL};

	unsigned int vertexAmount, startIndex, modelID;

	float blendFactor[4];
	blendFactor[0] = 0.0f;
	blendFactor[1] = 0.0f;
	blendFactor[2] = 0.0f;
	blendFactor[3] = 0.0f;

	UINT stride = sizeof(BBUI);
	UINT offset = 0;


	immediateContext->PSSetShaderResources(0,1,&nullSRV[0]);
	immediateContext->OMSetRenderTargets(1, &renderTargetView, depthStencilView);

	immediateContext->RSSetViewports(1, &viewPort);


	immediateContext->RSSetState(rasterizerBackface);

	immediateContext->OMSetDepthStencilState(depthStencilStateDisable, 0);
	
	D3D11_MAPPED_SUBRESOURCE updateData;
	ZeroMemory( &updateData, sizeof( updateData ) );

	immediateContext->Map(uiBufferDynamic, 0, D3D11_MAP_WRITE_DISCARD, 0, &updateData);
	memcpy( updateData.pData, &objectCore->uiBillboards[0], sizeof(BBUI)* objectCore->uiBillboards.size() );
	immediateContext->Unmap(uiBufferDynamic, 0);

	immediateContext->OMSetBlendState(blendEnable, blendFactor, 0xffffffff);
	immediateContext->IASetVertexBuffers( 0, 1, &uiBufferDynamic, &stride, &offset );
	immediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_POINTLIST);
	techniques.at( getTechIDByName( "techUI" ) )->useTechnique();
	immediateContext->IASetInputLayout(uiLayout);

	cbWorld.world			= Matrix(	1,0,0,0,
									0,1,0,0,
									0,0,1,0,
									0,0,0,1	);

	cbWorld.worldInv		= Matrix(	1,0,0,0,
									0,1,0,0,
									0,0,1,0,
									0,0,0,1	);
	updateCBWorld( cbWorld );

	immediateContext->PSSetShaderResources( 0,4, &textures.at( 41 ) );
	
	vertexAmount	= objectCore->gui.size() + 1;
	startIndex		= 1;

	immediateContext->Draw( vertexAmount, startIndex );

	//--------------------------------------------------------------------------------
	//                                     Text
	//--------------------------------------------------------------------------------
	stride = sizeof( BBFont );
	offset = 0;

	immediateContext->RSSetState(rasterizerBackface);

	D3D11_MAPPED_SUBRESOURCE textData;
	ZeroMemory( &textData, sizeof( textData ) );

	immediateContext->Map(textBufferDynamic, 0, D3D11_MAP_WRITE_DISCARD, 0, &textData);
	memcpy( textData.pData, &objectCore->fontBillboards[0], sizeof(BBFont)* objectCore->fontBillboards.size() );
	immediateContext->Unmap(textBufferDynamic, 0);

	immediateContext->IASetVertexBuffers( 0, 1, &textBufferDynamic, &stride, &offset );
	immediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_POINTLIST);
	techniques.at( getTechIDByName( "techFont" ) )->useTechnique();
	immediateContext->IASetInputLayout(fontLayout);
	immediateContext->PSSetShaderResources( 0,1,&textures.at( 4 ) );
	objectCore->fontBillboards.clear();
	for(unsigned int i = 0; i < objectCore->optionList.size(); i++)
	{
		startIndex			= objectCore->fontBillboards.size();
		objectCore->optionList.at(i).appendTextToData();
		vertexAmount		= objectCore->fontBillboards.size() - startIndex;

		objectCore->optionList.at(i).updateCB();

		immediateContext->Draw( vertexAmount, startIndex );
	}
}

void GraphicsDX11::useShaderResourceViews(ID3D11ShaderResourceView **views, int startSlot, int numberofViews)
{
	immediateContext->PSSetShaderResources(startSlot,numberofViews,views);
}

void GraphicsDX11::feedInstanceBuffer( std::vector<PerInstance> perInstance)
{

}

GraphicsDX11::~GraphicsDX11()
{
	SAFE_RELEASE(device);
	SAFE_RELEASE(immediateContext);

	SAFE_RELEASE(renderTargetView);

	SAFE_RELEASE(sceneTex);
	SAFE_RELEASE(sceneRenderTargetView);
	SAFE_RELEASE(sceneShaderResource);

	SAFE_RELEASE(reflTex);
	SAFE_RELEASE(reflRenderTargetView);
	SAFE_RELEASE(reflShaderResource);

	SAFE_RELEASE(depthStencilTex);
	SAFE_RELEASE(depthStencilView);
	SAFE_RELEASE(depthStencilResource);

	SAFE_RELEASE(blendEnable);
	SAFE_RELEASE(blendDisable);

	SAFE_RELEASE(depthStencilStateEnable);
	SAFE_RELEASE(depthStencilStateDisable);

	SAFE_RELEASE(rasterizerBackface);
	SAFE_RELEASE(rasterizerFrontface);

	SAFE_RELEASE(samplerLinear);

	SAFE_RELEASE(simpleInputLayout);
	SAFE_RELEASE(uiLayout);
	SAFE_RELEASE(fontLayout);

	SAFE_RELEASE(swapChain);

	SAFE_RELEASE(cbWorld);
	SAFE_RELEASE(cbCameraMove);
	SAFE_RELEASE(cbOnce);
	SAFE_RELEASE(cbFont);

	SAFE_RELEASE(vBufferStatic);
	SAFE_RELEASE(uiBufferDynamic);
	SAFE_RELEASE(vBufferDynamic);
	SAFE_RELEASE(textBufferDynamic);
	SAFE_RELEASE(instBuffer);

	//techniques
	for(unsigned int i = 0; i < techniques.size(); i++)
		SAFE_DELETE( techniques.at(i) );

	//textures
	//for(unsigned int i = 0; i < textures.size(); i++)
	//	SAFE_RELEASE( textures.at(i) );

	//cbuffers
	SAFE_RELEASE(cbWorld);
	SAFE_RELEASE(cbOnce);
	SAFE_RELEASE(cbCameraMove);
	SAFE_RELEASE(cbFont);

	TechniqueHLSL::cleanUp();
}

void GraphicsDX11::getTextureArray(std::vector<ID3D11ShaderResourceView*> *_textureArray)
{
	Resources::LoadHandler *loader = Resources::LoadHandler::getInstance();

	ID3D11ShaderResourceView*	texture = NULL;

	D3DX11_IMAGE_LOAD_INFO loadInfo;
	ZeroMemory( &loadInfo, sizeof(D3DX11_IMAGE_LOAD_INFO) );
	loadInfo.BindFlags = D3D10_BIND_SHADER_RESOURCE;
	loadInfo.Format = DXGI_FORMAT_BC1_UNORM;

	for(unsigned int i = 0; i < loader->getTextureSize();i++)
	{
		D3DX11CreateShaderResourceViewFromFile( device, loader->getTexture(i)->getFilePath(), NULL, NULL, &texture, NULL );

		_textureArray->push_back(texture);
	}

}

#endif // _WIN32
