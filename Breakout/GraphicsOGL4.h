#ifndef _WIN32

#ifndef _GRAPHICSOGL4_H_
#define _GRAPHICSOGL4_H_

#include <stdio.h>
#include <stdlib.h>
#include <GL/glew.h>
#include <GL/glfw.h>
#include <vector>

#include "ObjectCore.h"
#include "LoadHandler.h"
#include "ProgramGLSL.h"
#include "linearalgebra.h"

struct tempBBUI
{
	Vec2 pos;
	Vec2 size;
};

class GraphicsOGL4
{
private:
	std::vector<GLuint> *textures;

	Logic::ObjectCore	*objectCore;
	// static vertex buffer
	GLuint vertexBufferStatic;
	// dynamic buffer for UI elements
	GLuint uiBufferDynamic;

	GLuint	modelMatID,
			modelInvMatID,
			projMatID,
			viewMatID,
			viewSkybox,
			projSkybox;

	GLuint diffuseTexID,
			skyboxTexID;

    ProgramGLSL *program,
				*billboardProgram,
				*skyboxProgram;

	// VAO
	GLuint  VertexArrayID,
            modelMatrixBlockID; /// This will be used later, when known how it works..

	Resources::LoadHandler *lh;

	static GraphicsOGL4 *instance;
	int feedStaticBufferData(std::vector<Vertex> vertexpoints);

	GraphicsOGL4();

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

	void initVertexBuffer();
	int feedUIBufferData(std::vector<tempBBUI> _points);

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

	void useTechnique(unsigned int index);
	void useTexture(int index);

	~GraphicsOGL4();
};

#endif // ! _GRAPHICSOGL4_H_

#endif // ! _WIN32
