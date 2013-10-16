#ifndef BAJSAPA

#include "GraphicsOGL4.h"
#include "Resource.h"

GraphicsOGL4 *GraphicsOGL4::instance = NULL;

struct Matrices
{
    Matrix *view;
    Matrix *viewInverse;
    Matrix *projection;
    Matrix *projectionInverse;
}matrices;


GraphicsOGL4::GraphicsOGL4()
{
    // Initiate matrices so that they all are pointing to
    matrices.view = NULL;
    matrices.viewInverse = NULL;
    matrices.projection = NULL;
    matrices.projectionInverse = NULL;

	glGenVertexArrays(1, &VertexArrayID);
	glBindVertexArray(VertexArrayID);

	// generate buffers
	glGenBuffers(1, &vertexBufferStatic);
	glGenBuffers(1, &uiBufferDynamic);
	glGenBuffers(1, &textBufferDynamic);

	//compile shader programs
	program = new ProgramGLSL("simple", /*"/home/torrebjorne/Documents/GitHub/Breakout/Breakout/*/"shaders/glsl/vsSimple.glsl", "", /*"/home/torrebjorne/Documents/GitHub/Breakout/Breakout/*/"shaders/glsl/fsSimple.glsl");

	billboardProgram = new ProgramGLSL("billy", "shaders/glsl/vsBBUI.glsl", "shaders/glsl/gsBBUI.glsl", "shaders/glsl/fsBBUI.glsl");
	
	skyboxProgram = new ProgramGLSL("skybox", "shaders/glsl/vsSkybox.glsl", "", "shaders/glsl/fsSkybox.glsl");

	fontProgram = new ProgramGLSL("fonz", "shaders/glsl/vsFont.glsl", "shaders/glsl/gsFont.glsl", "shaders/glsl/fsFont.glsl");

	lh = Resources::LoadHandler::getInstance();
	textures = getTextures();

	modelMatID		= glGetUniformLocation(program->getProgramID(), "model");
	modelInvMatID	= glGetUniformLocation(program->getProgramID(), "modelInvTrans");
	projMatID		= glGetUniformLocation(program->getProgramID(), "projection");
	viewMatID		= glGetUniformLocation(program->getProgramID(), "view");

	initConstantBuffers();

	projSkybox		= glGetUniformLocation(skyboxProgram->getProgramID(), "projection");
	viewSkybox		= glGetUniformLocation(skyboxProgram->getProgramID(), "view");

	diffuseTexID = glGetUniformLocation(program->getProgramID(), "textureSampler");
	skyboxTexID = glGetUniformLocation(skyboxProgram->getProgramID(), "textureSampler");
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

}

void GraphicsOGL4::draw()
{

	// local variables
	unsigned int vertexAmount, startIndex, modelID;
	CBWorld cb;
	
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
	
	useMatrices();

	useTexture(objectCore->skybox->getTextureID());

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
	
	//use projection matrices and stuff.. will be removed
	useMatrices();

	// bind static vertex buffer. holds all data for static objects
	glBindBuffer(GL_ARRAY_BUFFER, vertexBufferStatic);

	// set buffer attribute layout
	useStandardVertexAttribLayout();

	// use texture for ball
	useTexture(objectCore->ball->getTextureID());

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
	
	useTexture(objectCore->pad->getTextureID());
	
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
	
		useTexture(objectCore->bricks[i]->getTextureID());
	
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
	//                            billboard
	//---------------------------------------------------------------------------------
	billboardProgram->useProgram();
	
	feedUIBufferData();
	glBindBuffer(GL_ARRAY_BUFFER, uiBufferDynamic);
	
	useBillboardVertexAttribLayout();
	
	useTexture(9);
	
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
	
	useTexture(8);
	
	vertexAmount		= objectCore->testText->getTextSize();
	startIndex			= objectCore->testText->getVBStartIndex();
	
	objectCore->testText->updateCB();

	glDrawArrays(GL_POINTS, startIndex, vertexAmount);

	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	
	glDisable (GL_BLEND);
	glEnable (GL_DEPTH_TEST); 
	//----------------------------------------------------------------------------------
	// disable vertex attributes 
	// (maybe should be in function, so that not to many of few attributes are disabled...)

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

//will be dynamic buffer or what!?
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
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		}
		else
		{
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		}
		// When MAGnifying the image (no bigger mipmap available), use LINEAR filtering
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		// When MINifying the image, use a LINEAR blend of two mipmaps, each filtered LINEARLY too
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
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
void	GraphicsOGL4::useTechnique(unsigned int index)
{

}
void	GraphicsOGL4::draw(unsigned int startIndex, unsigned int vertexAmount)
{

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

void GraphicsOGL4::useTexture(int _index)
{
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, textures->at(_index));

	glUniform1i(0, 0);
}

void GraphicsOGL4::updateModelMatrix(Matrix *_model)
{
	glUniformMatrix4fv(modelMatID, 1, GL_FALSE, &_model->r[0][0]);
}

void GraphicsOGL4::updateModelInvTransMatrix(Matrix *_modelinvtrans)
{
	glUniformMatrix4fv(modelInvMatID, 1, GL_FALSE, &_modelinvtrans->r[0][0]);
}

void GraphicsOGL4::updateViewMatrix(Matrix *_view)
{
    matrices.view = _view;
}

void GraphicsOGL4::updateProjectionMatrix(Matrix *_projection)
{
    matrices.projection = _projection;
}

void GraphicsOGL4::updateProjectionInverseMatrix(Matrix *_projectionInverse)
{
    matrices.projectionInverse = _projectionInverse;
}

void GraphicsOGL4::updateViewInverseMatrix(Matrix *_viewInverse)
{
    matrices.viewInverse = _viewInverse;
}

void GraphicsOGL4::useMatrices()
{
    //this has to be changed.... not good to have to ask for id for every new program...
    // should have shared blocks/buffers
	
	if(matrices.view)
	{
		glUniformMatrix4fv(viewMatID, 1, GL_FALSE, &matrices.view->r[0][0]);
		glUniformMatrix4fv(viewSkybox, 1, GL_FALSE, &matrices.view->r[0][0]);
	}
	if(matrices.projection)
	{
		glUniformMatrix4fv(projMatID, 1, GL_FALSE, &matrices.projection->r[0][0]);
		glUniformMatrix4fv(projSkybox, 1, GL_FALSE, &matrices.projection->r[0][0]);
	}
}
#endif // !BAJSAPA
