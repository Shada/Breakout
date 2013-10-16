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
	// dynamic buffer for UI elements
	GLuint uiBufferDynamic;
	// dynamic buffer for text
	GLuint textBufferDynamic;

	//matrixIDs
	GLuint	modelMatID,
			modelInvMatID,
			projMatID,
			viewMatID,
			viewSkybox,
			projSkybox;

	//texture IDs
	GLuint diffuseTexID,
			skyboxTexID;

	//ConstantBuffers
	GLuint	cbCameraOnce,
			cbCameraMove,
			cbWorld,
			cbFont,
			cbCameraOnceFont;

    ProgramGLSL *program,
				*billboardProgram,
				*skyboxProgram,
				*fontProgram;

	// VAO
	GLuint  VertexArrayID,
            modelMatrixBlockID; /// This will be used later, when known how it works..

	Resources::LoadHandler *lh;

	static GraphicsOGL4 *instance;
	int feedStaticBufferData(std::vector<Vertex> vertexpoints);

	GraphicsOGL4();
	
	void initConstantBuffers();

public:
	void draw();

	static GraphicsOGL4 *getInstance();

	void	setObjectCore(Logic::ObjectCore *objectCore) { this->objectCore = objectCore; }
	
	int		getTechIDByName(const char *name);
	std::vector<GLuint>* getTextures();

	void	draw(unsigned int startIndex, unsigned int vertexAmount);
	void	updateCBOnce(CBOnce cb);
	void	updateCBCameraMove(CBCameraMove cb);
	void	updateCBWorld(CBWorld cb);
	void	updateCBFont(CBFont cb);

	void initVertexBuffer();
	void feedUIBufferData();
	void feedTextBufferData();

	void updateModelMatrix(Matrix *model);
	void updateModelInvTransMatrix(Matrix *modelinvtrans);
	void updateViewMatrix(Matrix *view);
	void updateViewInverseMatrix(Matrix *viewInverse);
	void updateProjectionMatrix(Matrix *projection);
	void updateProjectionInverseMatrix(Matrix *projectionInverse);

	// maybe a little different later, this is temporary
	void useMatrices();

	/** Using standard vertex layout with Position, normal and texCoord **/
	void useStandardVertexAttribLayout();
	/** Using billboard vertex layout with Position and size **/
	void useBillboardVertexAttribLayout();
	/** Using font vertex layout with Position and texvalues **/
	void useFontVertexAttribLayout();

	void useTechnique(unsigned int index);
	void useTexture(int index);

	~GraphicsOGL4();
};

#endif // ! _GRAPHICSOGL4_H_

#endif // ! BAJSAPA
