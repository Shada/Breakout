#pragma once
#include "Resource.h"
#include <d3d11.h>
#include <D3DX11.h>
#include <string>
#include <vector>
struct VSstruct
{
	ID3D11VertexShader* vertexShader;
	std::string name;
};
struct HSstruct
{
	ID3D11HullShader* hullShader;
	std::string name;
};
struct DSstruct
{
	ID3D11DomainShader* domainShader;
	std::string name;
};
struct GSstruct
{
	ID3D11GeometryShader* geometryShader;
	std::string name;
};
struct PSstruct
{
	ID3D11PixelShader* pixelShader;
	std::string name;
};
class TechniqueHLSL
{
private:
	static std::vector<VSstruct>	*vertexShaders;
	static std::vector<HSstruct>	*hullShaders;
	static std::vector<DSstruct>	*domainShaders;
	static std::vector<GSstruct>	*geometryShaders;
	static std::vector<PSstruct>	*pixelShaders;
	int								insertVertexShader(std::string shaderDir, std::string shaderName);
	int								insertHullShader(std::string shaderDir, std::string shaderName);
	int								insertDomainShader(std::string shaderDir, std::string shaderName);
	int								insertGeometryShader(std::string shaderDir, std::string shaderName);
	int								insertPixelShader(std::string shaderDir, std::string shaderName);
	int								vertexShaderIndex;
	int								hullShaderIndex;
	int								domainShaderIndex;
	int								geometryShaderIndex;
	int								pixelShaderIndex;

	ID3D11Device					*device;
	ID3DBlob*						vertexBlob;
public:
	TechniqueHLSL(ID3D11Device *device, std::string vsPath, std::string vs, std::string gsPath,std::string gs, std::string psPath, std::string ps);
	TechniqueHLSL(ID3D11Device *device, std::string vsPath, std::string vs, std::string hsPath,std::string hs,std::string dsPath,std::string ds, std::string psPath, std::string ps);
	~TechniqueHLSL();


	static void cleanUp();
	void useTechnique();

	LPVOID getInputSignature()		{ return vertexBlob->GetBufferPointer(); }
	SIZE_T getInputSignatureSize()	{ return vertexBlob->GetBufferSize(); }
};

