#include "Particles.h"
#include "GraphicsDX11.h"


	//float globalTime;
 //   float elapsedTime;

Particles::	Particles(ID3D11Device *_device, char _shader[256] , Vec3 _startpos, Vec3 _startspeed, float _radie, Vec3 _radieDirection, int _particleTexID, Vec4 _frameGravity, float _lifeTime, float _stages, int _particlesPerStage, Vec4 _color,Vec4 _fadeColor,int _randomDir)
{
	GraphicsDX11* g = GraphicsDX11::getInstance();

	device = _device;

	First = true;
	Finished = false;
	count = 0;

	texID = _particleTexID;

	effectVariables.globalTime = 0;
	effectVariables.elapsedTime = 0;

	effectVariables.frameGravity = _frameGravity;
	effectVariables.particlesPerStage = _particlesPerStage;
	effectVariables.color = _color;
	effectVariables.fadeColor = _fadeColor;
	effectVariables.startPos = _startpos;
	effectVariables.startSpeed = _startspeed;
	effectVariables.stages = _stages;
	effectVariables.radie = _radie;
	effectVariables.radieDirections = _radieDirection;
	effectVariables.randDir = _randomDir;
	effectVariables.shellLife = _lifeTime;



	Initialize(_shader,_particleTexID);
	createBuffer(_startpos);
	g->updateCBParticles(effectVariables);

}


Particles::~Particles(void)
{
}


void Particles::Initialize(char _shader[256] , int _particleTexID)
{
	GraphicsDX11* g = GraphicsDX11::getInstance();

	const D3D11_INPUT_ELEMENT_DESC particleLayout[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TIMER", 0, DXGI_FORMAT_R32_FLOAT, 0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TYPE", 0, DXGI_FORMAT_R32_UINT, 0, 28, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};

	const int particleNumElements = sizeof(particleLayout)/sizeof(particleLayout[0]);

	device->CreateInputLayout( particleLayout, particleNumElements,g->getTechByName("particleAdvance")->getInputSignature(), g->getTechByName("particleAdvance")->getInputSignatureSize(), &ParticleVertexLayout );



	createRandomTexture();
}


void Particles::update(float dt)
{
	GraphicsDX11* g = GraphicsDX11::getInstance();
	g->getTechByName("particleAdvance")->useTechnique();

	// Set Effects Parameters
	effectVariables.elapsedTime = dt;
	effectVariables.globalTime += dt;

	g->updateCBParticles(effectVariables);


	g->immediateContext->IASetInputLayout( ParticleVertexLayout );
	// Set IA parameters
    ID3D11Buffer* pBuffers[1];
    if( First )
        pBuffers[0] = ParticleStart;
    else
        pBuffers[0] = ParticleDrawFrom;


    UINT stride[1] = { sizeof( PARTICLE_VERTEX ) };
    UINT offset[1] = { 0 };
    g->immediateContext->IASetVertexBuffers( 0, 1, pBuffers, &stride[0], &offset[0] );
    g->immediateContext->IASetPrimitiveTopology( D3D11_PRIMITIVE_TOPOLOGY_POINTLIST );

    // Point to the correct output buffer
    pBuffers[0] = ParticleStreamTo;
    g->immediateContext->SOSetTargets( 1, pBuffers, offset );

   


    // Draw
	
	g->immediateContext->GSSetShaderResources(0,1,&RandomTexRV);


        if( First )
		{
            g->immediateContext->Draw( 1, 0 );
			First = false;
		}
        else
            g->immediateContext->DrawAuto();

    // Get back to normal
    pBuffers[0] = NULL;
    g->immediateContext->SOSetTargets( 1, pBuffers, offset );

    // Swap particle buffers
    ID3D11Buffer* pTemp = ParticleDrawFrom;
    ParticleDrawFrom = ParticleStreamTo;
    ParticleStreamTo = pTemp;

	
	
}

void Particles::draw()
{
	GraphicsDX11* g = GraphicsDX11::getInstance();
	g->getTechByName("particleDraw")->useTechnique();
	//g->updateCBCameraMove();
	


    // Set IA parameters
    ID3D11Buffer* pBuffers[1] = { ParticleDrawFrom };
    UINT stride[1] = { sizeof( PARTICLE_VERTEX ) };
    UINT offset[1] = { 0 };
    g->immediateContext->IASetVertexBuffers( 0, 1, pBuffers, stride, offset );
    g->immediateContext->IASetPrimitiveTopology( D3D11_PRIMITIVE_TOPOLOGY_POINTLIST );
 

	g->immediateContext->IASetInputLayout( ParticleVertexLayout );
   
	// Draw
	g->immediateContext->GSSetShaderResources(0,1,g->getTextureByID(texID));


    g->immediateContext->DrawAuto();
   // g->immediateContext->Draw(MAX_PARTICLES,0);
    
	float blendFactor[] = {0,0,0,0};
	g->immediateContext->OMSetBlendState(NULL,blendFactor,0xffffffff);
}


void Particles::createRandomTexture()
{
	GraphicsDX11* g = GraphicsDX11::getInstance();

	Vec4 randomValues[1024];

	for(int i = 0; i < 1024; ++i)
	{
		randomValues[i].x = ((float)rand() / (float)RAND_MAX + (float)rand() / (float)RAND_MAX) - 1;
		randomValues[i].y = ((float)rand() / (float)RAND_MAX + (float)rand() / (float)RAND_MAX) - 1;
		randomValues[i].z = ((float)rand() / (float)RAND_MAX + (float)rand() / (float)RAND_MAX) - 1;
		randomValues[i].w = ((float)rand() / (float)RAND_MAX + (float)rand() / (float)RAND_MAX) - 1;
	}

    D3D11_SUBRESOURCE_DATA initData;
    initData.pSysMem = randomValues;
	initData.SysMemPitch = 1024*sizeof(Vec4);
    initData.SysMemSlicePitch = 1024*sizeof(Vec4);
	//
	// Create the texture.
	//
    D3D11_TEXTURE1D_DESC texDesc;
    texDesc.Width = 1024;
    texDesc.MipLevels = 1;
    texDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
    texDesc.Usage = D3D11_USAGE_IMMUTABLE;
    texDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
    texDesc.CPUAccessFlags = 0;
    texDesc.MiscFlags = 0;
    texDesc.ArraySize = 1;

	ID3D11Texture1D* randomTex = 0;
    device->CreateTexture1D(&texDesc, &initData, &randomTex);
	//
	// Create the resource view.
	//
    D3D11_SHADER_RESOURCE_VIEW_DESC viewDesc;
	viewDesc.Format = texDesc.Format;
    viewDesc.ViewDimension = D3D10_SRV_DIMENSION_TEXTURE1D;
    viewDesc.Texture1D.MipLevels = texDesc.MipLevels;
	viewDesc.Texture1D.MostDetailedMip = 0;
	
	device->CreateShaderResourceView(randomTex, &viewDesc, &RandomTexRV);

	SAFE_RELEASE(randomTex);
}

void Particles::createBuffer( Vec3 pos)
{
	HRESULT hr;
	 D3D11_BUFFER_DESC vbdesc =
    {
        1 * sizeof( PARTICLE_VERTEX ),
        D3D11_USAGE_DEFAULT,
        D3D11_BIND_VERTEX_BUFFER,
        0,
        0
    };

	 //vbdesc.MiscFlags = D3D11_CPU_ACCESS_WRITE;


    D3D11_SUBRESOURCE_DATA vbInitData;
    ZeroMemory( &vbInitData, sizeof( D3D11_SUBRESOURCE_DATA ) );

    PARTICLE_VERTEX vertStart =
    {
        Vec3(11,22,33),
        Vec3( 55, 66, 77 ),
        float( 555 ),
        UINT( 0 ),
    }; 
    vbInitData.SysMemPitch = sizeof( PARTICLE_VERTEX );
    vbInitData.pSysMem = &vertStart;

    hr = device->CreateBuffer( &vbdesc, &vbInitData, &ParticleStart );

    vbdesc.ByteWidth = MAX_PARTICLES * sizeof( PARTICLE_VERTEX );
    vbdesc.BindFlags |= D3D11_BIND_STREAM_OUTPUT;
    hr = device->CreateBuffer( &vbdesc, NULL, &ParticleDrawFrom );
    hr = device->CreateBuffer( &vbdesc, NULL, &ParticleStreamTo );
}
