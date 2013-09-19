#include "Gameplay.h"
#include "GraphicsDX11.h"


Logic::Gameplay::Gameplay(/*Map*/)
{
	//tolka Map och skapa object enligt den
}

Logic::Gameplay::~Gameplay()
{
	for(unsigned int i = 0; i < models.size(); i++)
		SAFE_DELETE(models[i]);
	models.clear();
}
void Logic::Gameplay::initVertexBuffer()
{
	std::vector<Vertex> vertices;
	int start = vertices.size();
	for(unsigned int i = 0; i < models.size(); i++)
	{
		models[i]->setStartIndex(start);
		vertices.insert(vertices.end(), models[i]->getData()->begin(), models[i]->getData()->end());
		start += models[i]->getData()->size();
	}
#ifdef _WIN32
	GraphicsDX11::getInstance()->createVBufferStatic(vertices);
#else
	//linux stuff
#endif // _WIN32
}
int Logic::Gameplay::Update(/*tid*/)
{
	return 0;
}