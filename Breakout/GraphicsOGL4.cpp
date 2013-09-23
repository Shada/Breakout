#ifndef _WIN32

#include "GraphicsOGL4.h"
#include "Resource.h"

GraphicsOGL4 *GraphicsOGL4::instance = NULL;

GraphicsOGL4::GraphicsOGL4()
{

	glGenVertexArrays(1, &VertexArrayID);
	glBindVertexArray(VertexArrayID);

	// generate the buffer and store the id in vertexBuffer var
	glGenBuffers(1, &vertexBufferStatic);

	tech = new TechniqueGLSL("simple", "/home/torrebjorne/Documents/GitHub/Breakout/Breakout/shaders/glsl/vsSimple.glsl", "", "/home/torrebjorne/Documents/GitHub/Breakout/Breakout/shaders/glsl/fsSimple.glsl");
}

GraphicsOGL4 *GraphicsOGL4::getInstance()
{
	if(!instance)
	{
		instance = new GraphicsOGL4();
	}
	return instance;
}

void GraphicsOGL4::draw(GLuint _vertexBufferID, int _startIndex, int _numVerts)
{
	// do some buffer magic
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, _vertexBufferID);

	// TODO: must find out some way to make this fancy
	glVertexAttribPointer(
		0,			// attribute 0. Must match layout in shader
		3,			// size
		GL_FLOAT,	// type
		GL_FALSE,	// normalized?
		0,			// stride
		(void*)0	// array buffer offset
		);

    // tech at certain position.. Send it in? or use from object... we'll see!!!
    tech->useTechnique();
	//do some drawing
	glDrawArrays(GL_TRIANGLES, _startIndex, _numVerts); // startindex = 0, num verts = 3

	glDisableVertexAttribArray(0);
}

int GraphicsOGL4::feedStaticBufferData(std::vector<float> _vertexpoints, int _numVertices)
{
	// bind vertex buffer to GPU
	glBindBuffer(GL_ARRAY_BUFFER, vertexBufferStatic);

	// feed data to buffer
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * _numVertices, &_vertexpoints[0], GL_STATIC_DRAW);

	return 0; // will need to be calculated based on how much there are already
}

GraphicsOGL4::~GraphicsOGL4()
{
	// delete stored buffer from opengl
	glDeleteBuffers(1, &vertexBufferStatic);
	glDeleteVertexArrays(1, &VertexArrayID);
}

#endif // !_WIN32
