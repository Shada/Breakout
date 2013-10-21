#ifndef BAJSAPA

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
	// dynamic buffer for text
	GLuint textBufferDynamic;

	//texture IDs
	GLuint	diffuseTexID,
			skyboxTexID,
			texSceneID,
			texDepthID,
			reflMapID,
			heightMapID,
			normMapID,
			foamMapID,
			quadTextureID;

	//ConstantBuffers
	GLuint	cbCameraOnce,
			cbCameraMove,
			cbWorld,
			cbFont,
			cbCameraOnceFont,
			cbWater,
			cbWaterOnce;

	//renderTargets and depthBuffers
	GLuint	reflFrameBuffer,
			sceneFrameBuffer,
			reflRenderTarget,
			sceneRenderTarget,
			waterDepthBuffer;

	// Programs
    ProgramGLSL *program,
				*billboardProgram,
				*skyboxProgram,
				*fontProgram,
				*waterProgram,
				*skyboxReflProgram,
				*quadProgram,
				*reflProgram;

	// VAO
	GLuint  VertexArrayID;

	Resources::LoadHandler *lh;

	static GraphicsOGL4 *instance;
	int feedStaticBufferData(std::vector<Vertex> vertexpoints);

	GraphicsOGL4();
	
	void initPrograms();
	void initConstantBuffers();
	void initUniforms();
	void initRenderTargetsAndDepthBuffers();

public:
	void draw();

	static GraphicsOGL4 *getInstance();

	void	setObjectCore(Logic::ObjectCore *objectCore) { this->objectCore = objectCore; }
	
	int		getTechIDByName(const char *name);
	std::vector<GLuint>* getTextures();

	void	updateCBOnce(CBOnce cb);
	void	updateCBCameraMove(CBCameraMove cb);
	void	updateCBWorld(CBWorld cb);
	void	updateCBFont(CBFont cb);
	void	updateCBWater(CBWater cb);
	void	updateCBWaterOnce(CBWaterOnce cb);

	void initVertexBuffer();
	void feedUIBufferData();
	void feedTextBufferData();

	/** Using standard vertex layout with Position, normal and texCoord **/
	void useStandardVertexAttribLayout();
	/** Using billboard vertex layout with Position and size **/
	void useBillboardVertexAttribLayout();
	/** Using font vertex layout with Position and texvalues **/
	void useFontVertexAttribLayout();

	/* use a texture in the existing texture list */
	void useTexture(int textureIndex, GLuint samplerID);
	/* use a texture outside the texture list. for example a render target */
	void useTexture(GLuint textureIndex, GLuint _samplerID, GLint v0);

	~GraphicsOGL4();
};

#endif // ! _GRAPHICSOGL4_H_

#endif // ! BAJSAPA
