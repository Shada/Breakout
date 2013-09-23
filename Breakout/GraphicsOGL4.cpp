#ifndef _WIN32

#include "GraphicsOGL4.h"
#include "Resource.h"

GraphicsOGL4 *GraphicsOGL4::instance = NULL;

GraphicsOGL4::GraphicsOGL4()
{

	glGenVertexArrays(1, &VertexArrayID);
	glBindVertexArray(VertexArrayID);

	// generate the buffer and store the id in vertexBufferStatic var
	glGenBuffers(1, &vertexBufferStatic);

	program = new ProgramGLSL("simple", "/home/torrebjorne/Documents/GitHub/Breakout/Breakout/shaders/glsl/vsSimple.glsl", "", "/home/torrebjorne/Documents/GitHub/Breakout/Breakout/shaders/glsl/fsSimple.glsl");

	//Get blockID for matrices
	modelMatrixBlockID = glGetUniformBlockIndex(program->getProgramID(), "ModelMatrixBlock");
}

GraphicsOGL4::~GraphicsOGL4()
{
	// delete stored buffer from opengl
	glDeleteBuffers(1, &vertexBufferStatic);
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
    Matrix model;

	model[0][0] = 0.5f;
    //Test if can update model matrix
	updateModelMatrix(&model, program->getProgramID());


	// do some buffer magic
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, vertexBufferStatic);

	// use vertex layout
	useStandardVertexAttribLayout();

    // tech at certain position.. Send it in? or use from object... we'll see!!!
    program->useProgram();
	//do some drawing
	glDrawArrays(GL_TRIANGLES, _startIndex, _numVerts); // startindex = 0, num verts = 3

	glDisableVertexAttribArray(0);
}

int GraphicsOGL4::feedStaticBufferData(std::vector<float> _vertexpoints, int _size)
{
	// bind vertex buffer to GPU
	glBindBuffer(GL_ARRAY_BUFFER, vertexBufferStatic);

	// feed data to buffer
	glBufferData(GL_ARRAY_BUFFER, _size, &_vertexpoints[0], GL_STATIC_DRAW);

	return 0; // will need to be calculated based on how much there are already
}

void GraphicsOGL4::useStandardVertexAttribLayout()
{
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 8, (void*)0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 8, (void*)3);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 8, (void*)6);
}

void GraphicsOGL4::updateModelMatrix(Matrix *_model, GLuint _programID)
{
    //this has to be changed.... not good to have to ask for id for every new program...
    GLuint matrixID = glGetUniformLocation(_programID, "model");
    glUniformMatrix4fv(matrixID, 1, GL_FALSE, &_model->r[0][0]);
}

void GraphicsOGL4::updateViewMatrix(Matrix *_view)
{
    GLuint matrixID = glGetUniformLocation(program->getProgramID(), "view");
    glUniformMatrix4fv(matrixID, 1, GL_FALSE, &_view->r[0][0]);
}

void GraphicsOGL4::updateProjectionMatrix(Matrix *_projection)
{
    GLuint matrixID = glGetUniformLocation(program->getProgramID(), "projection");
    glUniformMatrix4fv(matrixID, 1, GL_FALSE, &_projection->r[0][0]);
}

void GraphicsOGL4::updateProjectionInverseMatrix(Matrix *_projectionInverse)
{
    GLuint matrixID = glGetUniformLocation(program->getProgramID(), "projectioninverse");
    glUniformMatrix4fv(matrixID, 1, GL_FALSE, &_projectionInverse->r[0][0]);
}

void GraphicsOGL4::updateViewInverseMatrix(Matrix *_viewInverse)
{
    GLuint matrixID = glGetUniformLocation(program->getProgramID(), "viewinverse");
    glUniformMatrix4fv(matrixID, 1, GL_FALSE, &_viewInverse->r[0][0]);
}

#endif // !_WIN32
