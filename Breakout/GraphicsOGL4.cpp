#ifndef BAJSAPA

#include "GraphicsOGL4.h"
#include "Resource.h"
#include <string>

GraphicsOGL4 *GraphicsOGL4::instance = NULL;

GraphicsOGL4::GraphicsOGL4()
{
	glGenVertexArrays(1, &VertexArrayID);
	glBindVertexArray(VertexArrayID);

	// generate buffers
	glGenBuffers(1, &vertexBufferStatic);
	glGenBuffers(1, &uiBufferDynamic);
	glGenBuffers(1, &textBufferDynamic);

	initPrograms();

	lh = Resources::LoadHandler::getInstance();

	textures = getTextures();
	initConstantBuffers();
	initUniforms();
	initRenderTargetsAndDepthBuffers();
}


GraphicsOGL4::~GraphicsOGL4()
{
	ProgramGLSL::cleanup();
	// delete stored buffer from opengl
	glDeleteBuffers(1, &vertexBufferStatic);
	glDeleteBuffers(1, &uiBufferDynamic);
	SAFE_DELETE(program);
	SAFE_DELETE(skyboxProgram);
	SAFE_DELETE(billboardProgram);
	SAFE_DELETE(fontProgram);
	SAFE_DELETE(reflProgram);
	SAFE_DELETE(waterProgram);
	SAFE_DELETE(quadProgram);
	SAFE_DELETE(skyboxReflProgram);

	glDeleteVertexArrays(1, &VertexArrayID);
	for(unsigned int i = 0; i < textures->size(); i++)
		glDeleteTextures(1, &textures->at(i));
	SAFE_DELETE(textures);
}

GraphicsOGL4 *GraphicsOGL4::getInstance()
{
	if(!instance)
	{
		instance = new GraphicsOGL4();
	}
	return instance;
}


void GraphicsOGL4::draw()
{

	// local variables
	unsigned int vertexAmount, startIndex, modelID;
	CBWorld cb;

	//-----------------------------------------------------------------------------
	//-----------------------------------------------------------------------------
	//                              REFLECTIONS
	//-----------------------------------------------------------------------------
	//-----------------------------------------------------------------------------
	// draw to custom framebuffer (reflection)
	glBindFramebuffer(GL_FRAMEBUFFER, reflFrameBuffer);
	glViewport(0,0,SCRWIDTH,SCRHEIGHT);
	
	//clear screen
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//--------------------------------------------------------------------------------
	//									  Skybox
	//--------------------------------------------------------------------------------
	glDisable(GL_DEPTH_TEST);
    glCullFace(GL_FRONT);
	skyboxReflProgram->useProgram();

	// bind static vertex buffer. holds all data for static objects
	glBindBuffer(GL_ARRAY_BUFFER, vertexBufferStatic);

	// set buffer attribute layout
	useStandardVertexAttribLayout();
	
	useTexture(objectCore->skybox->getTextureID(), skyboxTexID);

	modelID = objectCore->skybox->getModelID();

	vertexAmount	= lh->getModel( modelID )->getVertexAmount();
	startIndex		= lh->getModel( modelID )->getStartIndex();

	glDrawArrays(GL_TRIANGLES, startIndex, vertexAmount);
	
    glCullFace(GL_BACK);
    glEnable(GL_DEPTH_TEST);
	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(2);
	//--------------------------------------------------------------------------------
	//                                    Ball(s)
	//--------------------------------------------------------------------------------
	
	//send in world matrix
	cb.world = objectCore->ball->getWorld();
	cb.worldInv = objectCore->ball->getWorldInv();
	updateCBWorld(cb);

	// use default program. program id might need to be fetched from object? 
	reflProgram->useProgram();

	// bind static vertex buffer. holds all data for static objects
	glBindBuffer(GL_ARRAY_BUFFER, vertexBufferStatic);

	// set buffer attribute layout
	useStandardVertexAttribLayout();

	// use texture for ball
	useTexture(objectCore->ball->getTextureID(), diffuseTexID);

	// get modelid for ball.
	modelID = objectCore->ball->getModelID();

	// fetch vertexamout and startid for ball model
	vertexAmount	= lh->getModel( modelID )->getVertexAmount();
	startIndex		= lh->getModel( modelID )->getStartIndex();
	
	// draw ball
	glDrawArrays(GL_TRIANGLES, startIndex, vertexAmount);
	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(2);
	////--------------------------------------------------------------------------------
	////                                     Pad
	////--------------------------------------------------------------------------------
	cb.world = objectCore->pad->getWorld();
	cb.worldInv = objectCore->pad->getWorldInv();
	updateCBWorld(cb);

	// bind static vertex buffer. holds all data for static objects
	glBindBuffer(GL_ARRAY_BUFFER, vertexBufferStatic);

	// set buffer attribute layout
	useStandardVertexAttribLayout();
	
	useTexture(objectCore->pad->getTextureID(), diffuseTexID);
	
	modelID			= objectCore->pad->getModelID();

	vertexAmount	= lh->getModel( modelID )->getVertexAmount();
	startIndex		= lh->getModel( modelID )->getStartIndex();
	
	glDrawArrays(GL_TRIANGLES, startIndex, vertexAmount);
	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(2);
	//--------------------------------------------------------------------------------
	//                                     bricks
	//--------------------------------------------------------------------------------
	
	for(unsigned int i = 0; i < objectCore->bricks.size(); i++)
	{
		cb.world = objectCore->bricks[i]->getWorld();
		cb.worldInv = objectCore->bricks[i]->getWorldInv();
		updateCBWorld(cb);
	
		// bind static vertex buffer. holds all data for static objects
		glBindBuffer(GL_ARRAY_BUFFER, vertexBufferStatic);
		// set buffer attribute layout
		useStandardVertexAttribLayout();
	
		useTexture(objectCore->bricks[i]->getTextureID(), diffuseTexID);
	
		modelID				= objectCore->bricks.at(i)->getModelID();
		vertexAmount		= lh->getModel( modelID )->getVertexAmount();
		startIndex			= lh->getModel( modelID )->getStartIndex();
	
		glDrawArrays(GL_TRIANGLES, startIndex, vertexAmount);
		// disable vertex attributes 
		// (maybe should be in function, so that not to many of few attributes are disabled...)
		glDisableVertexAttribArray(0);
		glDisableVertexAttribArray(1);
		glDisableVertexAttribArray(2);
	}
	
	

	//----------------------------------------------------------------------------------------
	//----------------------------------------------------------------------------------------
	//                    second pass draw normally to scene
	//----------------------------------------------------------------------------------------
	//----------------------------------------------------------------------------------------
	
	// draw to custom framebuffer (scene)
	glBindFramebuffer(GL_FRAMEBUFFER, sceneFrameBuffer);
	glViewport(0,0,SCRWIDTH,SCRHEIGHT);
	
	//clear screen
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	//--------------------------------------------------------------------------------
	//									  Skybox
	//--------------------------------------------------------------------------------
	glDisable(GL_DEPTH_TEST);
    glCullFace(GL_FRONT);
	skyboxProgram->useProgram();
	
	// bind static vertex buffer. holds all data for static objects
	glBindBuffer(GL_ARRAY_BUFFER, vertexBufferStatic);
	
	// set buffer attribute layout
	useStandardVertexAttribLayout();
	
	useTexture(objectCore->skybox->getTextureID(), skyboxTexID);
	
	modelID = objectCore->skybox->getModelID();
	
	vertexAmount	= lh->getModel( modelID )->getVertexAmount();
	startIndex		= lh->getModel( modelID )->getStartIndex();
	
	glDrawArrays(GL_TRIANGLES, startIndex, vertexAmount);
	
    glCullFace(GL_BACK);
    glEnable(GL_DEPTH_TEST);
	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(2);
	//--------------------------------------------------------------------------------
	//                                    Ball(s)
	//--------------------------------------------------------------------------------
	
	//send in world matrix
	cb.world = objectCore->ball->getWorld();
	cb.worldInv = objectCore->ball->getWorldInv();
	updateCBWorld(cb);
	
	// use default program. program id might need to be fetched from object? 
	program->useProgram();

	// bind static vertex buffer. holds all data for static objects
	glBindBuffer(GL_ARRAY_BUFFER, vertexBufferStatic);
	
	// set buffer attribute layout
	useStandardVertexAttribLayout();
	
	// use texture for ball
	useTexture(objectCore->ball->getTextureID(), diffuseTexID);
	
	// get modelid for ball.
	modelID = objectCore->ball->getModelID();
	
	// fetch vertexamout and startid for ball model
	vertexAmount	= lh->getModel( modelID )->getVertexAmount();
	startIndex		= lh->getModel( modelID )->getStartIndex();
	
	// draw ball
	glDrawArrays(GL_TRIANGLES, startIndex, vertexAmount);
	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(2);
	////--------------------------------------------------------------------------------
	////                                     Pad
	////--------------------------------------------------------------------------------
	cb.world = objectCore->pad->getWorld();
	cb.worldInv = objectCore->pad->getWorldInv();
	updateCBWorld(cb);
	
	// bind static vertex buffer. holds all data for static objects
	glBindBuffer(GL_ARRAY_BUFFER, vertexBufferStatic);
	
	// set buffer attribute layout
	useStandardVertexAttribLayout();
	
	useTexture(objectCore->pad->getTextureID(), diffuseTexID);
	
	modelID			= objectCore->pad->getModelID();
	
	vertexAmount	= lh->getModel( modelID )->getVertexAmount();
	startIndex		= lh->getModel( modelID )->getStartIndex();
	
	glDrawArrays(GL_TRIANGLES, startIndex, vertexAmount);
	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(2);
	//--------------------------------------------------------------------------------
	//                                     bricks
	//--------------------------------------------------------------------------------
	
	for(unsigned int i = 0; i < objectCore->bricks.size(); i++)
	{
		cb.world = objectCore->bricks[i]->getWorld();
		cb.worldInv = objectCore->bricks[i]->getWorldInv();
		updateCBWorld(cb);
	
		// bind static vertex buffer. holds all data for static objects
		glBindBuffer(GL_ARRAY_BUFFER, vertexBufferStatic);
		// set buffer attribute layout
		useStandardVertexAttribLayout();
	
		useTexture(objectCore->bricks[i]->getTextureID(), diffuseTexID);
	
		modelID				= objectCore->bricks.at(i)->getModelID();
		vertexAmount		= lh->getModel( modelID )->getVertexAmount();
		startIndex			= lh->getModel( modelID )->getStartIndex();
	
		glDrawArrays(GL_TRIANGLES, startIndex, vertexAmount);
		// disable vertex attributes 
		// (maybe should be in function, so that not to many of few attributes are disabled...)
		glDisableVertexAttribArray(0);
		glDisableVertexAttribArray(1);
		glDisableVertexAttribArray(2);
	}
	
	//---------------------------------------------------------------------------------
	//                              water
	//---------------------------------------------------------------------------------
	
	// change back to screen framebuffer
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glViewport(0, 0, SCRWIDTH, SCRHEIGHT);
	//clear screen
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	waterProgram->useProgram();

	useTexture(sceneRenderTarget, texSceneID, 0); // I have to find this shit..
	useTexture(waterDepthBuffer, texDepthID, 1);
	useTexture(reflRenderTarget, reflMapID, 2);
	useTexture(textures->at(11), heightMapID, 3);
	useTexture(textures->at(12), normMapID, 4);
	useTexture(textures->at(13), foamMapID, 5);

	glDrawArrays(GL_POINTS, 0, 1);
	
	// clear the depth
	glClear(GL_DEPTH_BUFFER_BIT);
	//---------------------------------------------------------------------------------
	//                            billboard
	//---------------------------------------------------------------------------------
	billboardProgram->useProgram();
	
	feedUIBufferData();
	glBindBuffer(GL_ARRAY_BUFFER, uiBufferDynamic);
	
	useBillboardVertexAttribLayout();
	
	useTexture(9, diffuseTexID);
	
	vertexAmount = objectCore->uiBillboards.size();
	startIndex	 = 0;
	
	glDrawArrays(GL_POINTS, startIndex, vertexAmount);
	
	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	//-----------------------------------------------------------------------------------
	//                             text
	//-----------------------------------------------------------------------------------
	fontProgram->useProgram();
	glDisable(GL_DEPTH_TEST);
	glEnable (GL_BLEND); 
	
	glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	
	feedTextBufferData();
	glBindBuffer(GL_ARRAY_BUFFER, textBufferDynamic);
	
	useFontVertexAttribLayout();
	
	useTexture(8, diffuseTexID);
	
	vertexAmount		= objectCore->testText->getTextSize();
	startIndex			= objectCore->testText->getVBStartIndex();
	
	objectCore->testText->updateCB();
	
	glDrawArrays(GL_POINTS, startIndex, vertexAmount);
	
	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	
	glDisable (GL_BLEND);
	glEnable (GL_DEPTH_TEST); 


	//----------------------------------------------------------------------------------
	//                                quad
	//----------------------------------------------------------------------------------
	
	//// draw to screen
	//glBindFramebuffer(GL_FRAMEBUFFER, 0);
	//glViewport(0, 0, SCRWIDTH, SCRHEIGHT);
	////clear screen
	//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	//
	//quadProgram->useProgram();
	//
	//useTexture(reflRenderTarget, quadTextureID);
	//
	//glDrawArrays(GL_POINTS, 0, 1);
	
	if(glCheckFramebufferStatus(GL_FRAMEBUFFER != GL_FRAMEBUFFER_COMPLETE))
		printf("WARNING: Something wrong with framebuffer!!!");
}

void GraphicsOGL4::initVertexBuffer()
{
	std::vector<Vertex> vertices;
	int start = vertices.size();
	int nModels = lh->getModelSize();
	for(int i = 0; i < nModels; i++)
	{
		lh->getModel(i)->setStartIndex(start);
		vertices.insert(vertices.end(), lh->getModel(i)->getData()->begin(), lh->getModel(i)->getData()->end());
		start += lh->getModel(i)->getData()->size();
	}
	feedStaticBufferData(vertices);
}

int GraphicsOGL4::feedStaticBufferData(std::vector<Vertex> _vertexpoints)
{
	// bind vertex buffer to GPU
	glBindBuffer(GL_ARRAY_BUFFER, vertexBufferStatic);
	// feed data to buffer
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * _vertexpoints.size(), &_vertexpoints[0], GL_STATIC_DRAW);

	return 0; // will need to be calculated based on how much there are already //....WTF!??!?! What the hell did I mean with this !?!?! 
}

void GraphicsOGL4::feedUIBufferData()
{
	std::vector<BBUI> _points = objectCore->uiBillboards;
	// bind vertex buffer to GPU
	glBindBuffer(GL_ARRAY_BUFFER, uiBufferDynamic);
	// feed data to buffer
	glBufferData(GL_ARRAY_BUFFER, sizeof(BBUI) * _points.size(), &_points[0], GL_DYNAMIC_DRAW);
}

void GraphicsOGL4::feedTextBufferData()
{
	std::vector<BBFont> _points = objectCore->fontBillboards;
	// bind vertex buffer to GPU
	glBindBuffer(GL_ARRAY_BUFFER, textBufferDynamic);
	// feed data to buffer
	glBufferData(GL_ARRAY_BUFFER, sizeof(BBFont) * _points.size(), &_points[0], GL_DYNAMIC_DRAW);
}

std::vector<GLuint> *GraphicsOGL4::getTextures()
{
	//bool TextureManager::LoadTexture(const char* filename, const unsigned int texID, GLenum image_format, GLint internal_format, GLint level, GLint border);
	GLuint gl_texID;
	std::vector<GLuint> *back = new std::vector<GLuint>();
	std::string s;

	for(unsigned int i = 0; i < lh->getTextureSize();i++)
	{

		//generate an OpenGL texture ID for this texture
		glGenTextures(1, &gl_texID);
		//bind to the new texture ID
		glBindTexture(GL_TEXTURE_2D, gl_texID);

		//if png, it has alpha
		if(lh->getTexture(i)->isTransparent())
		{
			//store the texture data for OpenGL use
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, lh->getTexture(i)->getWidth(), lh->getTexture(i)->getHeight(), 
						0, GL_BGRA, GL_UNSIGNED_BYTE, lh->getTexture(i)->getBits());
		}
		else
		{
			//store the texture data for OpenGL use
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, lh->getTexture(i)->getWidth(), lh->getTexture(i)->getHeight(), 
						0, GL_BGR, GL_UNSIGNED_BYTE, lh->getTexture(i)->getBits());
		}
		
		s = lh->getTexture(i)->getFilePath();
		if(s.substr(0, 6) == "Skybox")
		{
			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		}
		else
		{
			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		}
		// When MAGnifying the image (no bigger mipmap available), use LINEAR filtering
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		// When MINifying the image, use a LINEAR blend of two mipmaps, each filtered LINEARLY too
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		// Generate mipmaps, by the way.
		glGenerateMipmap(GL_TEXTURE_2D);
		back->push_back(gl_texID);
	}
	return back;
}

int		GraphicsOGL4::getTechIDByName(const char *name)
{
	return -1;
}

void	GraphicsOGL4::updateCBOnce(CBOnce cb)
{
	glBindBuffer(GL_UNIFORM_BUFFER, cbCameraOnce);
	glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(CBOnce), &cb);
	glBindBuffer(GL_UNIFORM_BUFFER, cbCameraOnceFont);
	glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(CBOnce), &cb);
	
}
void	GraphicsOGL4::updateCBCameraMove(CBCameraMove cb)
{
	glBindBuffer(GL_UNIFORM_BUFFER, cbCameraMove);
	glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(CBCameraMove), &cb);
}

void	GraphicsOGL4::updateCBWorld(CBWorld cb)
{
	glBindBuffer(GL_UNIFORM_BUFFER, cbWorld);
	glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(CBWorld), &cb);
}

void	GraphicsOGL4::updateCBFont(CBFont cb)
{
	glBindBuffer(GL_UNIFORM_BUFFER, cbFont);
	glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(CBFont), &cb);
}

void	GraphicsOGL4::updateCBWater(CBWater cb)
{
	glBindBuffer(GL_UNIFORM_BUFFER, cbWater);
	glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(CBWater), &cb);
}

void	GraphicsOGL4::updateCBWaterOnce(CBWaterOnce cb)
{
	glBindBuffer(GL_UNIFORM_BUFFER, cbWaterOnce);
	glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(CBWaterOnce), &cb);
}

void GraphicsOGL4::useStandardVertexAttribLayout()
{
	glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 8, (void*)0);
	glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 8, (void*)(sizeof(float) * 3));
	glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 8, (void*)(sizeof(float) * 6));
}

void GraphicsOGL4::useBillboardVertexAttribLayout()
{
	glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 4, (void*)0);
	glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 4, (void*)(sizeof(float) * 2));
}

void GraphicsOGL4::useFontVertexAttribLayout()
{
	glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 1, GL_FLOAT, GL_FALSE, sizeof(float) * 5, (void*)0);
	glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(float) * 5, (void*)sizeof(float));
}

void GraphicsOGL4::useTexture(int _index, GLuint _samplerID)
{
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, textures->at(_index));

	glUniform1i(_samplerID, 0);
}

void GraphicsOGL4::useTexture(GLuint _index, GLuint _samplerID, GLint _v0 )
{
	switch(_v0)
	{
		case 0:
		{
			glActiveTexture(GL_TEXTURE0);
			break;
		}
		case 1:
		{
			glActiveTexture(GL_TEXTURE1);
			break;
		}
		case 2:
		{
			glActiveTexture(GL_TEXTURE2);
			break;
		}
		case 3:
		{
			glActiveTexture(GL_TEXTURE3);
			break;
		}
		case 4:
		{
			glActiveTexture(GL_TEXTURE4);
			break;
		}
		case 5:
		{
			glActiveTexture(GL_TEXTURE5);
			break;
		}
	}
	glBindTexture(GL_TEXTURE_2D, _index);

	glUniform1i(_samplerID, _v0);
}

void GraphicsOGL4::initConstantBuffers()
{
	GLuint blockIndex;

	blockIndex			= glGetUniformBlockIndex(program->getProgramID(), "CameraOnce");
	glUniformBlockBinding(program->getProgramID(), blockIndex, 0);
	
	glGenBuffers(1, &cbCameraOnce);
	glBindBuffer(GL_UNIFORM_BUFFER, cbCameraOnce);
	
	glBufferData(GL_UNIFORM_BUFFER, sizeof(CBOnce), NULL, GL_DYNAMIC_DRAW);
	glBindBufferBase(GL_UNIFORM_BUFFER, 0, cbCameraOnce);
	
	blockIndex			= glGetUniformBlockIndex(program->getProgramID(), "CameraMove");
	glUniformBlockBinding(program->getProgramID(), blockIndex, 1);
	
	glGenBuffers(1, &cbCameraMove);
	glBindBuffer(GL_UNIFORM_BUFFER, cbCameraMove);
	
	glBufferData(GL_UNIFORM_BUFFER, sizeof(CBCameraMove), NULL, GL_DYNAMIC_DRAW);
	glBindBufferBase(GL_UNIFORM_BUFFER, 1, cbCameraMove);

	blockIndex			= glGetUniformBlockIndex(program->getProgramID(), "WorldBuffer");
	glUniformBlockBinding(program->getProgramID(), blockIndex, 2);

	glGenBuffers(1, &cbWorld);
	glBindBuffer(GL_UNIFORM_BUFFER, cbWorld);

	glBufferData(GL_UNIFORM_BUFFER, sizeof(CBWorld), NULL, GL_DYNAMIC_DRAW);
	glBindBufferBase(GL_UNIFORM_BUFFER, 2, cbWorld);
	
	blockIndex			= glGetUniformBlockIndex(fontProgram->getProgramID(), "FontBuffer");
	glUniformBlockBinding(fontProgram->getProgramID(), blockIndex, 3);
	
	glGenBuffers(1, &cbFont);
	glBindBuffer(GL_UNIFORM_BUFFER, cbFont);
	
	glBufferData(GL_UNIFORM_BUFFER, sizeof(CBFont), NULL, GL_DYNAMIC_DRAW);
	glBindBufferBase(GL_UNIFORM_BUFFER, 3, cbFont);
	
	blockIndex			= glGetUniformBlockIndex(fontProgram->getProgramID(), "CameraOnce");
	glUniformBlockBinding(fontProgram->getProgramID(), blockIndex, 4);
	
	glGenBuffers(1, &cbCameraOnceFont);
	glBindBuffer(GL_UNIFORM_BUFFER, cbCameraOnceFont);
	
	glBufferData(GL_UNIFORM_BUFFER, sizeof(CBOnce), NULL, GL_DYNAMIC_DRAW);
	glBindBufferBase(GL_UNIFORM_BUFFER, 4, cbCameraOnceFont);

	blockIndex			= glGetUniformBlockIndex(waterProgram->getProgramID(), "cbWater");
	glUniformBlockBinding(waterProgram->getProgramID(), blockIndex, 5);
	
	glGenBuffers(1, &cbWater);
	glBindBuffer(GL_UNIFORM_BUFFER, cbWater);
	
	glBufferData(GL_UNIFORM_BUFFER, sizeof(CBWater), NULL, GL_DYNAMIC_DRAW);
	glBindBufferBase(GL_UNIFORM_BUFFER, 5, cbWater);

	blockIndex			= glGetUniformBlockIndex(waterProgram->getProgramID(), "cbWaterOnce");
	glUniformBlockBinding(waterProgram->getProgramID(), blockIndex, 6);
	
	glGenBuffers(1, &cbWaterOnce);
	glBindBuffer(GL_UNIFORM_BUFFER, cbWaterOnce);
	
	glBufferData(GL_UNIFORM_BUFFER, sizeof(CBWaterOnce), NULL, GL_DYNAMIC_DRAW);
	glBindBufferBase(GL_UNIFORM_BUFFER, 6, cbWaterOnce);

}

void GraphicsOGL4::initPrograms()
{
	//compile shader programs
	program = new ProgramGLSL("simple", /*"/home/torrebjorne/Documents/GitHub/Breakout/Breakout/*/"shaders/glsl/vsSimple.glsl", "", /*"/home/torrebjorne/Documents/GitHub/Breakout/Breakout/*/"shaders/glsl/fsSimple.glsl");

	billboardProgram = new ProgramGLSL("billy", "shaders/glsl/vsBBUI.glsl", "shaders/glsl/gsBBUI.glsl", "shaders/glsl/fsBBUI.glsl");
	
	skyboxProgram = new ProgramGLSL("skybox", "shaders/glsl/vsSkybox.glsl", "", "shaders/glsl/fsSkybox.glsl");

	fontProgram = new ProgramGLSL("fonz", "shaders/glsl/vsFont.glsl", "shaders/glsl/gsFont.glsl", "shaders/glsl/fsFont.glsl");

	reflProgram = new ProgramGLSL("refl", "shaders/glsl/vsRefl.glsl", "", "shaders/glsl/fsRefl.glsl");

	waterProgram = new ProgramGLSL("water", "shaders/glsl/vsQuad.glsl", "shaders/glsl/gsQuad.glsl", "shaders/glsl/fsWater.glsl");

	quadProgram = new ProgramGLSL("quad", "shaders/glsl/vsQuad.glsl", "shaders/glsl/gsQuad.glsl", "shaders/glsl/fsQuad.glsl");

	skyboxReflProgram = new ProgramGLSL("quad", "shaders/glsl/vsSkyboxRefl.glsl", "", "shaders/glsl/fsSkybox.glsl");
}

void GraphicsOGL4::initUniforms()
{
	diffuseTexID = glGetUniformLocation(program->getProgramID(), "textureSampler");
	skyboxTexID = glGetUniformLocation(skyboxProgram->getProgramID(), "textureSampler");

	//textures for water
	texSceneID = glGetUniformLocation(waterProgram->getProgramID(), "texScene");
	texDepthID = glGetUniformLocation(waterProgram->getProgramID(), "texDepth");
	reflMapID = glGetUniformLocation(waterProgram->getProgramID(), "reflectionMap");
	heightMapID = glGetUniformLocation(waterProgram->getProgramID(), "heightMap");
	normMapID = glGetUniformLocation(waterProgram->getProgramID(), "normalMap");
	foamMapID = glGetUniformLocation(waterProgram->getProgramID(), "foamMap");

	quadTextureID = glGetUniformLocation(quadProgram->getProgramID(), "textureSampler");
}

void GraphicsOGL4::initRenderTargetsAndDepthBuffers()
{
	//reflection rendertarget
	// this is framebuffer
	reflFrameBuffer = 0;
	glGenFramebuffers(1, &reflFrameBuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, reflFrameBuffer);

	// this is rendertarget
	glGenTextures(1, &reflRenderTarget);

	// rendertarget gets bound
	glBindTexture(GL_TEXTURE_2D, reflRenderTarget);

	// empty image
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, SCRWIDTH, SCRHEIGHT, 0, GL_RGB, GL_UNSIGNED_BYTE, 0);

	// linear filter
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	

	// set rendertarget as attachment #0
	glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, reflRenderTarget, 0);

	//scene rendertarget
	// this is framebuffer
	sceneFrameBuffer = 0;
	glGenFramebuffers(1, &sceneFrameBuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, sceneFrameBuffer);

	// this is rendertarget
	glGenTextures(1, &sceneRenderTarget);

	// rendertarget gets bound
	glBindTexture(GL_TEXTURE_2D, sceneRenderTarget);

	// empty image
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, SCRWIDTH, SCRHEIGHT, 0, GL_RGB, GL_UNSIGNED_BYTE, 0);

	// linear filter
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	// this is depth texture
	glGenTextures(1, &waterDepthBuffer);
	glBindTexture(GL_TEXTURE_2D, waterDepthBuffer);
	glTexImage2D(GL_TEXTURE_2D, 0,GL_DEPTH_COMPONENT32, SCRWIDTH, SCRHEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, 0);
	
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	// set rendertarget as attachment #0
	glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, sceneRenderTarget, 0);
	glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, waterDepthBuffer, 0);
	

	// set list of draw buffers
	GLenum drawBuffers[2] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1 };
	glDrawBuffers(2, drawBuffers);
	

	if(glCheckFramebufferStatus(GL_FRAMEBUFFER != GL_FRAMEBUFFER_COMPLETE))
		printf("WARNING: Something wrong with framebuffer!!!");

}


#endif // !BAJSAPA
