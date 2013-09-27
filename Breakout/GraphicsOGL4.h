#ifndef _WIN32

#ifndef _GRAPHICSOGL4_H_
#define _GRAPHICSOGL4_H_

#include <stdio.h>
#include <stdlib.h>
<<<<<<< HEAD
#include <gl/glew.h>
#include <gl/glfw.h>
#include "GraphicsInterface.h"
#include "ObjectCore.h"
=======
#include <GL/glew.h>
#include <GL/glfw.h>
#include <vector>

#include "LoadHandler.h"
#include "ProgramGLSL.h"

#include "linearalgebra.h"


class GraphicsOGL4
{
private:
	Logic::ObjectCore	*objectCore;
	// static vertex buffer
	GLuint vertexBufferStatic;

    ProgramGLSL *program;

	// VAO
	GLuint  VertexArrayID,
            modelMatrixBlockID; /// This will be used later, when known how it works..

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
	void	setObjectCore(Logic::ObjectCore *objectCore) { this->objectCore = objectCore; }
	int		feedData(GLuint vertexBufferID, float vertexpoints[], int numVertices);
	int		getTechIDByName(const char *name);
	void	useTechnique(unsigned int index);
	void	draw(unsigned int startIndex, unsigned int vertexAmount);
	void	updateCBOnce(CBOnce cb);
	void	updateCBCameraMove(CBCameraMove cb);
	void	updateCBWorld(CBWorld cb);
	int feedStaticBufferData(std::vector<Vertex> vertexpoints);

	/** Using standard vertex layout with Position, normal and texCoord **/
	void useStandardVertexAttribLayout();

	void updateModelMatrix(Matrix *model);
	void updateViewMatrix(Matrix *view);
	void updateViewInverseMatrix(Matrix *viewInverse);
	void updateProjectionMatrix(Matrix *projection);
	void updateProjectionInverseMatrix(Matrix *projectionInverse);

	// maybe a little different later, this is temporary
	void useMatrices(GLuint programID);

	~GraphicsOGL4();
};

#endif // ! _GRAPHICSOGL4_H_

#endif // ! _WIN32
