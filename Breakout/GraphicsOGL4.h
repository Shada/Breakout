#ifndef _WIN32

#ifndef _GRAPHICSOGL4_H_
#define _GRAPHICSOGL4_H_

#include <stdio.h>
#include <stdlib.h>
#include <gl/glew.h>
#include <gl/glfw.h>

class GraphicsOGL4
{
private:
	// static vertex buffer
	GLuint vertexBufferStatic[2];

	// VAO
	GLuint VertexArrayID;

	static GraphicsOGL4 *instance;

	GraphicsOGL4();

public:

	void draw(GLuint vertexBufferID, int startIndex, int numVertices);

	static GraphicsOGL4 *getInstance();

	// this will be somewhat a problem.. Maybe need different definitions for different type of data structures? 
	// Not necessary if all models have same vertex structure, but will this be the case always? This must be researched
	// will not be vector in future.. Probably better to be vector of structs 
	/** [in] vertexBufferId - ID of the buffer that is to be fed with data	*
	 * [in] vertexPoints - an array with all vertex points					*
	 * [in] numVertices - number of vertices sent in						*
	 * [return] the start index of the data sent in							**/
	int feedData(GLuint vertexBufferID, float vertexpoints[], int numVertices);

	~GraphicsOGL4();
};

#endif // !_GRAPHICSOGL4_H_

#endif // !_WIN32