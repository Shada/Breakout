#ifndef BAJSAPA

#include "GraphicsOGL4.h"
#include "Resource.h"

GraphicsOGL4 *GraphicsOGL4::instance = NULL;

struct Matrices
{
    Matrix *model;
    Matrix *view;
    Matrix *viewInverse;
    Matrix *projection;
    Matrix *projectionInverse;
}matrices;

GraphicsOGL4::GraphicsOGL4()
{
    // Initiate matrices so that they all are pointing to
    matrices.model = NULL;
    matrices.view = NULL;
    matrices.viewInverse = NULL;
    matrices.projection = NULL;
    matrices.projectionInverse = NULL;

	glGenVertexArrays(1, &VertexArrayID);
	glBindVertexArray(VertexArrayID);

	// generate the buffer and store the id in vertexBufferStatic var
	glGenBuffers(1, &vertexBufferStatic);

	program = new ProgramGLSL("simple", /*"/home/torrebjorne/Documents/GitHub/Breakout/Breakout/*/"shaders/glsl/vsSimple.glsl", "", /*"/home/torrebjorne/Documents/GitHub/Breakout/Breakout/*/"shaders/glsl/fsSimple.glsl");

	//Get blockID for matrices
	///modelMatrixBlockID = glGetUniformBlockIndex(program->getProgramID(), "ModelMatrixBlock");
}

GraphicsOGL4::~GraphicsOGL4()
{
	ProgramGLSL::cleanup();
	// delete stored buffer from opengl
	glDeleteBuffers(1, &vertexBufferStatic);
	SAFE_DELETE(program);
	glDeleteVertexArrays(1, &VertexArrayID);
}

GraphicsOGL4 *GraphicsOGL4::getInstance()
{
	if(!instance)
	{
		instance = new GraphicsOGL4();
	}
	return instance;
}

void GraphicsOGL4::draw(int _startIndex, int _numVerts)
{

    // program at certain position.. Send it in? or use from object... we'll see!!!
    program->useProgram();


    Matrix model;
    //Test if can update model matrix
	updateModelMatrix(&model);
    useMatrices(program->getProgramID());


	glBindBuffer(GL_ARRAY_BUFFER, vertexBufferStatic);

	// use vertex layout
	useStandardVertexAttribLayout();


	//do some drawing
	glDrawArrays(GL_TRIANGLES, _startIndex, _numVerts); // startindex = 0, num verts = 3

	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(2);
}

void GraphicsOGL4::draw()
{
	unsigned int vertexAmount, startIndex, modelID;

	Resources::LoadHandler *lh = Resources::LoadHandler::getInstance();
	Matrix world;

	program->useProgram();


	useStandardVertexAttribLayout();

	glBindBuffer(GL_ARRAY_BUFFER, vertexBufferStatic);

	//--------------------------------------------------------------------------------
	//                                    Ball(s)
	//--------------------------------------------------------------------------------

	world = objectCore->ball->getWorld();
	updateModelMatrix(&world);

	useMatrices(program->getProgramID());

	modelID = objectCore->ball->getModelID();
	vertexAmount	= lh->getModel( modelID )->getVertexAmount();
	startIndex		= lh->getModel( modelID )->getStartIndex();

	glDrawArrays(GL_TRIANGLES, startIndex, vertexAmount);

	//--------------------------------------------------------------------------------
	//                                     Pad
	//--------------------------------------------------------------------------------

	world		= objectCore->pad->getWorld();
	updateModelMatrix(&world);
	
	useMatrices(program->getProgramID());

	modelID			= objectCore->pad->getModelID();
	vertexAmount	= lh->getModel( modelID )->getVertexAmount();
	startIndex		= lh->getModel( modelID )->getStartIndex();
	
	glDrawArrays(GL_TRIANGLES, startIndex, vertexAmount);
	
	//--------------------------------------------------------------------------------
	//                                     bricks
	//--------------------------------------------------------------------------------

	for(unsigned int i = 0; i < objectCore->bricks.size(); i++)
	{
		world		= objectCore->bricks.at(i)->getWorld();
		updateModelMatrix(&world);
	
		useMatrices(program->getProgramID());

		modelID				= objectCore->bricks.at(i)->getModelID();
		vertexAmount		= lh->getModel( modelID )->getVertexAmount();
		startIndex			= lh->getModel( modelID )->getStartIndex();

		glDrawArrays(GL_TRIANGLES, startIndex, vertexAmount);
	}


	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(2);
}

int GraphicsOGL4::feedStaticBufferData(std::vector<Vertex> _vertexpoints)
{
	// bind vertex buffer to GPU
	glBindBuffer(GL_ARRAY_BUFFER, vertexBufferStatic);
    int s = sizeof(Vertex) * _vertexpoints.size();
	// feed data to buffer
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * _vertexpoints.size(), &_vertexpoints[0], GL_STATIC_DRAW);

	return 0; // will need to be calculated based on how much there are already
}

void GraphicsOGL4::initVertexBuffer()
{
	Resources::LoadHandler *loader = Resources::LoadHandler::getInstance();
	std::vector<Vertex> vertices;
	int start = vertices.size();
	for(unsigned int i = 0; i < loader->getModelSize(); i++)
	{
		loader->getModel(i)->setStartIndex(start);
		vertices.insert(vertices.end(), loader->getModel(i)->getData()->begin(), loader->getModel(i)->getData()->end());
		start += loader->getModel(i)->getData()->size();
	}
	feedStaticBufferData(vertices);
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

}
void	GraphicsOGL4::updateCBCameraMove(CBCameraMove cb)
{

}
void	GraphicsOGL4::updateCBWorld(CBWorld cb)
{

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

void GraphicsOGL4::updateModelMatrix(Matrix *_model)
{
    matrices.model = _model;
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

void GraphicsOGL4::useMatrices(GLuint _programID)
{
    //this has to be changed.... not good to have to ask for id for every new program...
    // should have shared blocks/buffers

    GLuint matrixID = glGetUniformLocation(_programID, "model");
    glUniformMatrix4fv(matrixID, 1, GL_FALSE, &matrices.model->r[0][0]);

    matrixID = glGetUniformLocation(program->getProgramID(), "view");
    glUniformMatrix4fv(matrixID, 1, GL_FALSE, &matrices.view->r[0][0]);

    matrixID = glGetUniformLocation(program->getProgramID(), "viewinverse");
    glUniformMatrix4fv(matrixID, 1, GL_FALSE, &matrices.viewInverse->r[0][0]);

    matrixID = glGetUniformLocation(program->getProgramID(), "projection");
    glUniformMatrix4fv(matrixID, 1, GL_FALSE, &matrices.projection->r[0][0]);

    matrixID = glGetUniformLocation(program->getProgramID(), "projectioninverse");
    glUniformMatrix4fv(matrixID, 1, GL_FALSE, &matrices.projectionInverse->r[0][0]);

    matrices.model = NULL;
}
#endif // !BAJSAPA
