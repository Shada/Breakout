#ifndef BAJSAPA

#ifndef _GRAPHICSOGL4_H_
#define _GRAPHICSOGL4_H_

#include <stdio.h>
#include <stdlib.h>
#include <GL/glew.h>
#include <GL/glfw.h>
#include "ObjectCore.h"
#include <vector>

#include "LoadHandler.h"
#include "ProgramGLSL.h"

#include "linearalgebra.h"


class GraphicsOGL4
{
private:
	std::vector<GLuint> *textures;

	Logic::ObjectCore	*objectCore;
	// static vertex buffer
	GLuint vertexBufferStatic;

    ProgramGLSL *program;

	// VAO
	GLuint  VertexArrayID,
            modelMatrixBlockID; /// This will be used later, when known how it works..

	Resources::LoadHandler *lh;

	static GraphicsOGL4 *instance;
	int feedStaticBufferData(std::vector<Vertex> vertexpoints);

	GraphicsOGL4();

public:

	void draw(int startIndex, int numVertices);
	void draw();

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
	std::vector<GLuint>* getTextures();

	void	draw(unsigned int startIndex, unsigned int vertexAmount);
	void	updateCBOnce(CBOnce cb);
	void	updateCBCameraMove(CBCameraMove cb);
	void	updateCBWorld(CBWorld cb);

	void initVertexBuffer();


	void updateModelMatrix(Matrix *model);
	void updateModelInvTransMatrix(Matrix *modelinvtrans);
	void updateViewMatrix(Matrix *view);
	void updateViewInverseMatrix(Matrix *viewInverse);
	void updateProjectionMatrix(Matrix *projection);
	void updateProjectionInverseMatrix(Matrix *projectionInverse);

	// maybe a little different later, this is temporary
	void useMatrices(GLuint programID);

	/** Using standard vertex layout with Position, normal and texCoord **/
	void useStandardVertexAttribLayout();

	void useTechnique(unsigned int index);
	void useTexture(int index);

	~GraphicsOGL4();
};

#endif // ! _GRAPHICSOGL4_H_

#endif // ! BAJSAPA
