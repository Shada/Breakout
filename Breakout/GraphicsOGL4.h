#ifndef _WIN32

#ifndef _GRAPHICSOGL4_H_
#define _GRAPHICSOGL4_H_

#include <stdio.h>
#include <stdlib.h>
#include <GL/glew.h>
#include <GL/glfw.h>
#include <vector>

#include "ProgramGLSL.h"

#include "linearalgebra.h"

class GraphicsOGL4
{
private:
	// static vertex buffer
	GLuint vertexBufferStatic;

    ProgramGLSL *program;

	// VAO
	GLuint  VertexArrayID,
            modelMatrixBlockID;

	static GraphicsOGL4 *instance;

	GraphicsOGL4();

public:

	void draw(int startIndex, int numVertices);

	static GraphicsOGL4 *getInstance();

	// this will be somewhat a problem.. Maybe need different definitions for different type of data structures?
	// Not necessary if all models have same vertex structure, but will this be the case always? This must be researched
	// will not be vector in future.. Probably better to be vector of structs
	/** [in] vertexPoints - an array with all vertex points					*
	 * [in] size of vertex data in bytes						*
	 * [return] the start index of the data sent in							**/
	int feedStaticBufferData(std::vector<float> vertexpoints, int _size);

	/** Using standard vertex layout with Position, normal and texCoord **/
	void useStandardVertexAttribLayout();

	void updateModelMatrix(Matrix *model, GLuint programID);
	void updateViewMatrix(Matrix *view);
	void updateViewInverseMatrix(Matrix *viewInverse);
	void updateProjectionMatrix(Matrix *projection);
	void updateProjectionInverseMatrix(Matrix *projectionInverse);

	~GraphicsOGL4();
};

#endif // ! _GRAPHICSOGL4_H_

#endif // ! _WIN32
