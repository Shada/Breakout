#ifdef BAJSAPA

#include "TechniqueHLSL.h"

std::vector<VSstruct> *TechniqueHLSL::vertexShaders		= new std::vector<VSstruct>();
std::vector<HSstruct> *TechniqueHLSL::hullShaders		= new std::vector<HSstruct>();
std::vector<DSstruct> *TechniqueHLSL::domainShaders		= new std::vector<DSstruct>();
std::vector<GSstruct> *TechniqueHLSL::geometryShaders	= new std::vector<GSstruct>();
std::vector<PSstruct> *TechniqueHLSL::pixelShaders		= new std::vector<PSstruct>();

#include "GraphicsDX11.h"

TechniqueHLSL::TechniqueHLSL(ID3D11Device *device, std::string name, std::string vsPath, std::string vs, std::string gsPath,std::string gs, std::string psPath, std::string ps)
{
	this->device		= device;
	vertexBlob			= NULL;
	vertexShaderIndex	= insertVertexShader(vsPath,vs);
	hullShaderIndex		= -1;
	domainShaderIndex	= -1;
	geometryShaderIndex	= insertGeometryShader(gsPath,gs);
	pixelShaderIndex	= insertPixelShader(psPath,ps);
	this->name			= name;
}
TechniqueHLSL::TechniqueHLSL(ID3D11Device *device, std::string name, std::string vsPath, std::string vs, std::string hsPath,std::string hs,std::string dsPath,std::string ds, std::string psPath, std::string ps)
{
	this->device		= device;
	vertexBlob			= NULL;
	vertexShaderIndex	= insertVertexShader(vsPath,vs);
	hullShaderIndex		= insertHullShader(hsPath,hs);
	domainShaderIndex	= insertDomainShader(dsPath, ds);
	geometryShaderIndex	= -1;
	pixelShaderIndex	= insertPixelShader(psPath,ps);
	this->name			= name;
}
int TechniqueHLSL::insertVertexShader(std::string shaderDir, std::string shaderName)
{
	if(shaderDir.empty() || shaderName.empty() )
	{
		std::string text = "Technique tried to compile pixel shader with no name" ;
		MessageBox( NULL, text.c_str(),"Shader Error",MB_OK);
		PostQuitMessage(0);
		return -1;
	}
	std::string shaderPath = shaderDir + ":" + shaderName;
	for(unsigned int i=0; i < vertexShaders->size(); i++)
	{
		if(shaderPath.size() == vertexShaders->at(i).name.size())
		{
			if( vertexShaders->at(i).name == shaderPath )
			{
				return i;
			}
		}
	}
	
	HRESULT hr;
	GraphicsDX11* g = GraphicsDX11::getInstance();
	hr = g->compileShader(shaderDir.c_str(),shaderName.c_str(),"vs_5_0",&vertexBlob );
	if(FAILED(hr))
	{
		std::string text = "Failed to compile " + shaderName;
		MessageBox( NULL, text.c_str(),"Shader Error",MB_OK);
		PostQuitMessage(0);
		return -1;
	}
	ID3D11VertexShader* shader;
	hr = device->CreateVertexShader(vertexBlob->GetBufferPointer(), vertexBlob->GetBufferSize(), NULL, &shader);
	if( FAILED(hr) )
	{
		g->shader5Support = false;
		hr = g->compileShader(shaderDir.c_str(),shaderName.c_str(),"vs_4_0",&vertexBlob );
		if(FAILED(hr))
		{
			std::string text = "Failed to compile " + shaderName;
			MessageBox( NULL, text.c_str(),"Shader Error",MB_OK);
			PostQuitMessage(0);
			return -1;
		}
		hr = device->CreateVertexShader(vertexBlob->GetBufferPointer(), vertexBlob->GetBufferSize(), NULL, &shader);
		if(FAILED(hr))
		{
			std::string text = "Failed to compile " + shaderName;
			MessageBox( NULL, text.c_str(),"Shader Error",MB_OK);
			PostQuitMessage(0);
			return -1;
		}
		std::cout << "Failed to create " << shaderPath << " using shader model 5. Reverting to shader model 4." << std::endl;

	}
	VSstruct vs;
	vs.name = shaderPath;
	vs.vertexShader = shader;
	vertexShaders->push_back(vs);
	return vertexShaders->size()-1;
	
}
int TechniqueHLSL::insertHullShader(std::string shaderDir, std::string shaderName)
{
	if(shaderDir.empty() || shaderName.empty() )
	{
		return -1;
	}
	std::string shaderPath = shaderDir + ":" + shaderName;
	for(unsigned int i=0; i < hullShaders->size(); i++)
	{
		if(shaderPath.size() == hullShaders->at(i).name.size())
		{
			if( hullShaders->at(i).name == shaderPath )
			{
				return i;
			}
		}
	}
	ID3DBlob* shaderBlob = NULL;
	
	HRESULT hr;
	GraphicsDX11* g = GraphicsDX11::getInstance();
	hr = g->compileShader(shaderDir.c_str(),shaderName.c_str(),"hs_5_0",&shaderBlob );
	if(FAILED(hr))
	{
		std::string text = "Failed to compile " + shaderName;
		MessageBox( NULL, text.c_str(),"Shader Error",MB_OK);
		PostQuitMessage(0);
		return -1;
	}
	ID3D11HullShader* shader;
	hr = device->CreateHullShader(shaderBlob->GetBufferPointer(), shaderBlob->GetBufferSize(), NULL, &shader);
	if( FAILED(hr) )
	{
		hr = g->compileShader(shaderDir.c_str(),shaderName.c_str(),"hs_4_0",&shaderBlob );
		if(FAILED(hr))
		{
			std::string text = "Failed to compile " + shaderName;
			MessageBox( NULL, text.c_str(),"Shader Error",MB_OK);
			PostQuitMessage(0);
			return -1;
		}
		hr = device->CreateHullShader(shaderBlob->GetBufferPointer(), shaderBlob->GetBufferSize(), NULL, &shader);
		if(FAILED(hr))
		{
			std::string text = "Failed to create " + shaderName;
			MessageBox( NULL, text.c_str(),"Shader Error",MB_OK);
			PostQuitMessage(0);
			return -1;
		}
		std::cout << "Failed to create " << shaderPath << " using shader model 5. Reverting to shader model 4." << std::endl;
	}
	shaderBlob->Release();
	HSstruct hs;
	hs.name = shaderPath;
	hs.hullShader = shader;
	hullShaders->push_back(hs);
	return hullShaders->size()-1;
}
int TechniqueHLSL::insertDomainShader(std::string shaderDir, std::string shaderName)
{
	if(shaderDir.empty() || shaderName.empty() )
	{
		return -1;
	}
	std::string shaderPath = shaderDir + ":" + shaderName;
	for(unsigned int i=0; i < domainShaders->size(); i++)
	{
		if(shaderPath.size() == domainShaders->at(i).name.size())
		{
			if( domainShaders->at(i).name == shaderPath )
			{
				return i;
			}
		}
	}
	ID3DBlob* shaderBlob = NULL;
	
	HRESULT hr;
	GraphicsDX11* g = GraphicsDX11::getInstance();
	hr = g->compileShader(shaderDir.c_str(),shaderName.c_str(),"ds_5_0",&shaderBlob );
	if(FAILED(hr))
	{
		std::string text = "Failed to compile " + shaderName;
		MessageBox( NULL, text.c_str(),"Shader Error",MB_OK);
		PostQuitMessage(0);
		return -1;
	}
	ID3D11DomainShader* shader;
	hr = device->CreateDomainShader(shaderBlob->GetBufferPointer(), shaderBlob->GetBufferSize(), NULL, &shader);
	if( FAILED(hr) )
	{
		hr = g->compileShader(shaderDir.c_str(),shaderName.c_str(),"ds_4_0",&shaderBlob );
		if(FAILED(hr))
		{
			std::string text = "Failed to compile " + shaderName;
			MessageBox( NULL, text.c_str(),"Shader Error",MB_OK);
			PostQuitMessage(0);
			return -1;
		}
		hr = device->CreateDomainShader(shaderBlob->GetBufferPointer(), shaderBlob->GetBufferSize(), NULL, &shader);
		if(FAILED(hr))
		{
			std::string text = "Failed to create " + shaderName;
			MessageBox( NULL, text.c_str(),"Shader Error",MB_OK);
			PostQuitMessage(0);
			return -1;
		}
		std::cout << "Failed to create " << shaderPath << " using shader model 5. Reverting to shader model 4." << std::endl;
	}
	shaderBlob->Release();
	DSstruct ds;
	ds.name = shaderPath;
	ds.domainShader = shader;
	domainShaders->push_back(ds);
	return domainShaders->size()-1;
}
int TechniqueHLSL::insertGeometryShader(std::string shaderDir, std::string shaderName)
{
	if(shaderDir.empty() || shaderName.empty() )
	{
		return -1;
	}
	std::string shaderPath = shaderDir + ":" + shaderName;
	for(unsigned int i=0; i < geometryShaders->size(); i++)
	{
		if(shaderPath.size() == geometryShaders->at(i).name.size())
		{
			if( geometryShaders->at(i).name == shaderPath )
			{
				return i;
			}
		}
	}
	ID3DBlob* shaderBlob = NULL;
	
	HRESULT hr;
	GraphicsDX11* g = GraphicsDX11::getInstance();
	hr = g->compileShader(shaderDir.c_str(),shaderName.c_str(),"gs_5_0",&shaderBlob );
	if(FAILED(hr))
	{
		std::string text = "Failed to compile " + shaderName;
		MessageBox( NULL, text.c_str(),"Shader Error",MB_OK);
		PostQuitMessage(0);
		return -1;
	}
	ID3D11GeometryShader* shader;
	hr = device->CreateGeometryShader(shaderBlob->GetBufferPointer(), shaderBlob->GetBufferSize(), NULL, &shader);
	if( FAILED(hr) )
	{
		hr = g->compileShader(shaderDir.c_str(),shaderName.c_str(),"gs_4_0",&shaderBlob );
		if(FAILED(hr))
		{
			std::string text = "Failed to compile " + shaderName;
			MessageBox( NULL, text.c_str(),"Shader Error",MB_OK);
			PostQuitMessage(0);
			return -1;
		}
		hr = device->CreateGeometryShader(shaderBlob->GetBufferPointer(), shaderBlob->GetBufferSize(), NULL, &shader);
		if(FAILED(hr))
		{
			std::string text = "Failed to compile " + shaderName;
			MessageBox( NULL, text.c_str(),"Shader Error",MB_OK);
			PostQuitMessage(0);
			return -1;
		}
		std::cout << "Failed to create " << shaderPath << " using shader model 5. Reverting to shader model 4." << std::endl;
	}
	shaderBlob->Release();
	GSstruct gs;
	gs.name = shaderPath;
	gs.geometryShader = shader;
	geometryShaders->push_back(gs);
	return geometryShaders->size()-1;
	
}

int TechniqueHLSL::insertPixelShader(std::string shaderDir, std::string shaderName)
{
	if(shaderDir.empty() || shaderName.empty() )
	{
		std::string text = "Technique tried to compile pixel shader with no name" ;
		MessageBox( NULL, text.c_str(),"Shader Error",MB_OK);
		PostQuitMessage(0);
		return -1;
	}
	std::string shaderPath = shaderDir + ":" + shaderName;
	for(unsigned int i=0; i < pixelShaders->size(); i++)
	{
		if(shaderPath.size() == pixelShaders->at(i).name.size())
		{
			if( pixelShaders->at(i).name == shaderPath )
			{
				return i;
			}
		}
	}
	ID3DBlob* shaderBlob = NULL;
	
	HRESULT hr;
	GraphicsDX11* g = GraphicsDX11::getInstance();
	hr = g->compileShader(shaderDir.c_str(),shaderName.c_str(),"ps_5_0",&shaderBlob );
	if(FAILED(hr))
	{
		std::string text = "Failed to compile " + shaderName;
		MessageBox( NULL, text.c_str(),"Shader Error",MB_OK);
		PostQuitMessage(0);
		return -1;
	}
	ID3D11PixelShader* shader;
	hr = device->CreatePixelShader(shaderBlob->GetBufferPointer(), shaderBlob->GetBufferSize(), NULL, &shader);
	if( FAILED(hr) )
	{
		hr = g->compileShader(shaderDir.c_str(),shaderName.c_str(),"ps_4_0",&shaderBlob );
		if(FAILED(hr))
		{
			std::string text = "Failed to compile " + shaderName;
			MessageBox( NULL, text.c_str(),"Shader Error",MB_OK);
			PostQuitMessage(0);
			return -1;
		}
		hr = device->CreatePixelShader(shaderBlob->GetBufferPointer(), shaderBlob->GetBufferSize(), NULL, &shader);
		if(FAILED(hr))
		{
			std::string text = "Failed to compile " + shaderName;
			MessageBox( NULL, text.c_str(),"Shader Error",MB_OK);
			PostQuitMessage(0);
			return -1;
		}
		std::cout << "Failed to create " << shaderPath << " using shader model 5. Reverting to shader model 4." << std::endl;
	}
	shaderBlob->Release();
	PSstruct ps;
	ps.name = shaderPath;
	ps.pixelShader = shader;
	pixelShaders->push_back(ps);
	return pixelShaders->size()-1;
	
}

void TechniqueHLSL::useTechnique()
{
	GraphicsDX11* g = GraphicsDX11::getInstance();
	//set vertexshader for this technique
	//vertex shader is always set
	g->immediateContext->VSSetShader(vertexShaders->at(vertexShaderIndex).vertexShader,NULL,0);

	//set hullshader if there is one in this technique
	if(hullShaderIndex == -1)
		g->immediateContext->HSSetShader(NULL,NULL,0);
	else
		g->immediateContext->HSSetShader(hullShaders->at(hullShaderIndex).hullShader,NULL,0);

	//set domain sahder if there is one in this techinque
	if(domainShaderIndex == -1)
		g->immediateContext->DSSetShader(NULL,NULL,0);
	else
		g->immediateContext->DSSetShader(domainShaders->at(domainShaderIndex).domainShader,NULL,0);

	//set geometry shader if there is one in this technique
	if(geometryShaderIndex == -1)
		g->immediateContext->GSSetShader(NULL,NULL,0);
	else
		g->immediateContext->GSSetShader(geometryShaders->at(geometryShaderIndex).geometryShader,NULL,0);

	//set pixel shader, there is always one to set
	g->immediateContext->PSSetShader(pixelShaders->at(pixelShaderIndex).pixelShader,NULL,0);

}

void TechniqueHLSL::cleanUp()
{
	for(unsigned int i = 0; i < vertexShaders->size(); i++)
		SAFE_RELEASE(vertexShaders->at(i).vertexShader);

	for(unsigned int i = 0; i < hullShaders->size(); i++)
		SAFE_RELEASE(hullShaders->at(i).hullShader);

	for(unsigned int i = 0; i < domainShaders->size(); i++)
		SAFE_RELEASE(domainShaders->at(i).domainShader);
	
	for(unsigned int i = 0; i < geometryShaders->size(); i++)
		SAFE_RELEASE(geometryShaders->at(i).geometryShader);
	
	for(unsigned int i = 0; i < pixelShaders->size(); i++)
		SAFE_RELEASE(pixelShaders->at(i).pixelShader);

	SAFE_DELETE(vertexShaders);
	SAFE_DELETE(hullShaders);
	SAFE_DELETE(domainShaders);
	SAFE_DELETE(geometryShaders);
	SAFE_DELETE(pixelShaders);
}

TechniqueHLSL::~TechniqueHLSL()
{
	if(vertexBlob)
		vertexBlob->Release();
}

#endif // BAJSAPA