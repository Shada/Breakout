#ifndef WINDOWS

#include "GraphicsOGL4.h"
#include "Resource.h"

GraphicsOGL4 *GraphicsOGL4::instance = NULL;

GraphicsOGL4::GraphicsOGL4()
{

	glGenVertexArrays(1, &VertexArrayID);
	glBindVertexArray(VertexArrayID);

	// generate the buffer and store the id in vertexBuffer var
	glGenBuffers(2, vertexBufferStatic);
}

GraphicsOGL4 *GraphicsOGL4::getInstance()
{
	if(!instance)
	{
		instance = new GraphicsOGL4();
	}
	return instance;
}

void GraphicsOGL4::draw(GLuint vertexBufferID, int startIndex, int numVerts)
{
	// do some buffer magic
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBufferID);
	glVertexAttribPointer(
		0,			// attribute 0. Must match layout in shader
		3,			// size
		GL_FLOAT,	// type
		GL_FALSE,	// normalized?
		0,			// stride
		(void*)0	// array buffer offset
		);
		
	//do some drawing
	glDrawArrays(GL_TRIANGLES, 0, 3); // startindex = 0, num verts = 3

	glDisableVertexAttribArray(0);
}

int GraphicsOGL4::feedData(GLuint vertexBufferID, float vertexpoints[9], int numVertices)
{
	// bind vertex buffer to GPU
	glBindBuffer(GL_ARRAY_BUFFER, vertexBufferID);

	// feed data to buffer
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * numVertices, vertexpoints, GL_STATIC_DRAW);

	return 0; // will need to be calculated based on how much there are already
}

GraphicsOGL4::~GraphicsOGL4()
{
	// delete stored buffer from opengl
	glDeleteBuffers(2, vertexBufferStatic);
	glDeleteVertexArrays(1, &VertexArrayID);
}

#endif // !_WIN32