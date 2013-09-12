#ifdef _WIN32

#include "GraphicsDX11.h"

GraphicsDX11	*GraphicsDX11::instance = NULL;

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
	blendEnable					= NULL;
	depthStencilStateEnable		= NULL;
	depthStencilStateDisable	= NULL;
	rasterizerBackface			= NULL;
	rasterizerFrontface			= NULL;
	samplerLinear				= NULL;
	shader5Support				= true;
}

void GraphicsDX11::init(HWND *hWnd)
{
	HRESULT hr = S_OK;

	RECT	rc;
	GetClientRect( *hWnd, &rc );
	UINT width = rc.right - rc.left;
    UINT height = rc.bottom - rc.top;

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
		D3D_FEATURE_LEVEL_11_1,
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
    sd.BufferDesc.RefreshRate.Numerator = 60;
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

	// Setup the viewport
    viewPort.Width		= (FLOAT)width;
    viewPort.Height		= (FLOAT)height;
    viewPort.MinDepth		= 0.0f;
    viewPort.MaxDepth		= 1.0f;
    viewPort.TopLeftX		= 0;
    viewPort.TopLeftY		= 0;
	immediateContext->RSSetViewports( 1, &viewPort );


	D3D11_INPUT_ELEMENT_DESC simpleLayout[] = 
	{
		{ "POSITION",	0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0,					D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL",		0, DXGI_FORMAT_R32G32B32_FLOAT, 0, sizeof(float) * 3,	D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD",	0, DXGI_FORMAT_R32G32_FLOAT,	0, sizeof(float)* 6,	D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};

	/*hr = device->CreateInputLayout(simpleLayout, ARRAYSIZE(simpleLayout), techModel->getInputSignature(),techModel->getInputSignatureSize(), &simpleInputLayout);
	if(FAILED(hr))
	{
		MessageBox( NULL, "Failed to create simpleInputLayout","GraphicsDX11 Error",MB_OK);
		return;
	}*/

	D3D11_INPUT_ELEMENT_DESC simpleLayoutInst[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, sizeof(float) * 3, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, sizeof(float) * 6, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "PERINST", 0, DXGI_FORMAT_R32G32B32_FLOAT, 1, 0, D3D11_INPUT_PER_INSTANCE_DATA, 1 },
		{ "PERINST", 1, DXGI_FORMAT_R32_FLOAT, 1, sizeof(float) * 3, D3D11_INPUT_PER_INSTANCE_DATA, 1 }
	};
	/*hr = device->CreateInputLayout(simpleLayoutInst, ARRAYSIZE(simpleLayoutInst), techInstModel->getInputSignature(),techInstModel->getInputSignatureSize(), &instanceLayout);
	if(FAILED(hr))
	{
		MessageBox( NULL, "Failed to create instancedInputLayout","GraphicsDX11 Error",MB_OK);
		return;
	}*/

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

	//create blendstates
	D3D11_BLEND_DESC blendDesc;
	ZeroMemory(&blendDesc,sizeof(blendDesc));
	blendDesc.AlphaToCoverageEnable						= FALSE;
	blendDesc.IndependentBlendEnable					= FALSE;
	blendDesc.RenderTarget[0].BlendEnable				= TRUE;
	blendDesc.RenderTarget[0].SrcBlend					= D3D11_BLEND_ONE;
    blendDesc.RenderTarget[0].DestBlend					= D3D11_BLEND_ONE;
    blendDesc.RenderTarget[0].BlendOp					= D3D11_BLEND_OP_ADD;
    blendDesc.RenderTarget[0].SrcBlendAlpha				= D3D11_BLEND_ONE;
    blendDesc.RenderTarget[0].DestBlendAlpha			= D3D11_BLEND_ZERO;
    blendDesc.RenderTarget[0].BlendOpAlpha				= D3D11_BLEND_OP_ADD;
    blendDesc.RenderTarget[0].RenderTargetWriteMask		= 0x0f;

	hr = device->CreateBlendState(&blendDesc,&blendEnable);
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
	float clearColor[4] = {r,g,b,1};
	immediateContext->ClearRenderTargetView(renderTargetView,clearColor);
}

void GraphicsDX11::presentSwapChain()
{
	swapChain->Present( 0, 0 );
}

GraphicsDX11::~GraphicsDX11()
{
	SAFE_RELEASE(device);
	SAFE_RELEASE(immediateContext);
	SAFE_RELEASE(renderTargetTex);
	SAFE_RELEASE(renderTargetView);
	SAFE_RELEASE(renderTargetResource);
	SAFE_RELEASE(depthStencilTex);
	SAFE_RELEASE(depthStencilView);
	SAFE_RELEASE(depthStencilResource);
	SAFE_RELEASE(blendEnable);
	SAFE_RELEASE(depthStencilStateEnable);
	SAFE_RELEASE(depthStencilStateDisable);
	SAFE_RELEASE(rasterizerBackface);
	SAFE_RELEASE(rasterizerFrontface);
	SAFE_RELEASE(samplerLinear);
	TechniqueHLSL::cleanUp();
}

#endif // _WIN32
