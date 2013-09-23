#include "ProgramGLSL.h"

std::vector<VSstruct> *ProgramGLSL::vertexShaders = new std::vector<VSstruct>();
std::vector<GSstruct> *ProgramGLSL::geometryShaders = new std::vector<GSstruct>();
std::vector<FSstruct> *ProgramGLSL::fragmentShaders = new std::vector<FSstruct>();
#include <fstream>

ProgramGLSL::ProgramGLSL(std::string _name, std::string _vsPath, std::string _gsPath, std::string _fsPath)
{
    name = _name;

    //1. insert VS
    vertexShaderIndex = insertVertexShader(_vsPath);

    //2. insert GS
    geometryShaderIndex = insertGeometryShader(_gsPath);

    //3. insert Fs
    fragmentShaderIndex = insertFragmentShader(_fsPath);

    //4. create program
    fprintf(stdout, "Creating Program\n");
    programID = glCreateProgram();

    //5. Attach shaders
    if(vertexShaderIndex >= 0)
    {
        printf("Attaching vertexshader %i to the %s technique.\n", vertexShaderIndex, name.c_str());
        glAttachShader(programID, vertexShaderIndex);
    }
    if(geometryShaderIndex >= 0)
    {
        printf("Attaching vertexshader %i to the %s technique.\n", geometryShaderIndex, name.c_str());
        glAttachShader(programID, geometryShaderIndex);
    }
    if(fragmentShaderIndex >= 0)
    {
        printf("Attaching vertexshader %i to the %s technique.\n", fragmentShaderIndex, name.c_str());
        glAttachShader(programID, fragmentShaderIndex);
    }

    //6. link program
    fprintf(stdout, "Linking Program\n");
    glLinkProgram(programID);

    GLint result = GL_FALSE;
    int infoLogLength;
    //7. check program
    glGetProgramiv(programID, GL_LINK_STATUS, &result);
    glGetProgramiv(programID, GL_INFO_LOG_LENGTH, &infoLogLength);
    std::vector<char> programErrorMessage(std::max(infoLogLength, int(1)));
    glGetShaderInfoLog(programID, infoLogLength, NULL, &programErrorMessage[0]);
    fprintf(stdout, "%s\n", &programErrorMessage[0]);
    if(result == GL_FALSE)
    {
        printf("something wrong with program");
    }
    //Maybe delete shaders
}

ProgramGLSL::~ProgramGLSL()
{

}

int ProgramGLSL::insertVertexShader(std::string _vsPath)
{
    //TODO: Check if vertex shader is already compiled

    if(_vsPath == "")
        return -1;
    //create vertex shader
    GLuint vs = glCreateShader(GL_VERTEX_SHADER);

    //read vertex shader code from file
    std::string vertexShaderCode;
    std::ifstream vertexShaderStream(_vsPath, std::ios::in);
    if(vertexShaderStream.is_open())
    {
        std::string line = "";
        while(getline(vertexShaderStream, line))
            vertexShaderCode += "\n" + line;
        vertexShaderStream.close();
    }
    else
    {
        printf("file %s not found\n", _vsPath.c_str());
    }

    GLint result = GL_FALSE;
    int infoLogLength;

    //Compile vertex shader
    printf("Compiling vertex shader: %s\n", _vsPath.c_str());
    const char *vertexSourcePtr = vertexShaderCode.c_str();
    glShaderSource(vs, 1, &vertexSourcePtr, NULL);
    glCompileShader(vs);

    //check vertex shader
    glGetShaderiv(vs, GL_COMPILE_STATUS, &result);
    glGetShaderiv(vs, GL_INFO_LOG_LENGTH, &infoLogLength);
    std::vector<char> vertexShaderErrorMessage(infoLogLength);
    fprintf(stdout, "Shader Message: %s\n", &vertexShaderErrorMessage[0]);

    if(result == GL_TRUE)
    {
        VSstruct v;
        v.name = _vsPath;
        v.vertexShader = vs;
        vertexShaders->push_back(v);

        printf("successfully compiled vertex shader: %s\n\n", _vsPath.c_str());

        return (int)vs;
    }
    else
    {
        printf("failed to compile vertex shader %s\n\n", _vsPath.c_str());
    }
    //return something else if failed? or maybe throw exeption... we'll see
    return -1;
}

int ProgramGLSL::insertGeometryShader(std::string _gsPath)
{
    if(_gsPath == "")
        return -1;
    //TODO:  check if geometry is  already compiled
    //create geometry shader
    GLuint gs = glCreateShader(GL_GEOMETRY_SHADER);

    //read geometry shader code from file
    std::string geometryShaderCode;
    std::ifstream geometryShaderStream(_gsPath, std::ios::in);
    if(geometryShaderStream.is_open())
    {
        std::string line = "";
        while(getline(geometryShaderStream, line))
            geometryShaderCode += "\n" + line;
        geometryShaderStream.close();
    }
    else
    {
        printf("file %s not found\n", _gsPath.c_str());
    }

    GLint result = GL_FALSE;
    int infoLogLength;

    //Compile geometry shader
    printf("Compiling geometry shader: %s\n", _gsPath.c_str());
    const char *geometrySourcePtr = geometryShaderCode.c_str();
    glShaderSource(gs, 1, &geometrySourcePtr, NULL);
    glCompileShader(gs);

    //check geometry shader
    glGetShaderiv(gs, GL_COMPILE_STATUS, &result);
    glGetShaderiv(gs, GL_INFO_LOG_LENGTH, &infoLogLength);
    std::vector<char> geometryShaderErrorMessage(infoLogLength);
    fprintf(stdout, "Shader Message: %s\n", &geometryShaderErrorMessage[0]);

    //return something else if failed? or maybe throw exeption... we'll see

    if(result == GL_TRUE)
    {
        GSstruct g;
        g.name = _gsPath;
        g.geometryShader = gs;
        geometryShaders->push_back(g);

        printf("successfully compiled geometry shader: %s\n\n", _gsPath.c_str());

        return (int)gs;
    }
    else
    {
        printf("failed to compile geometry shader %s\n\n", _gsPath.c_str());
    }
    return -1;
}

int ProgramGLSL::insertFragmentShader(std::string _fsPath)
{
    if(_fsPath == "")
        return -1;
    //TODO: Check if fragment shader is already compiled
    //create fragment shader
    GLuint fs = glCreateShader(GL_FRAGMENT_SHADER);

    //read fragment shader code from file
    std::string fragmentShaderCode;
    std::ifstream fragmentShaderStream(_fsPath, std::ios::in);
    if(fragmentShaderStream.is_open())
    {
        std::string line = "";
        while(getline(fragmentShaderStream, line))
            fragmentShaderCode += "\n" + line;
        fragmentShaderStream.close();
    }
    else
    {
        printf("file %s not found\n", _fsPath.c_str());
    }

    GLint result = GL_FALSE;
    int infoLogLength;

    //Compile fragment shader
    printf("Compiling fragment shader: %s\n", _fsPath.c_str());
    const char *fragmentSourcePtr = fragmentShaderCode.c_str();
    glShaderSource(fs, 1, &fragmentSourcePtr, NULL);
    glCompileShader(fs);

    //check fragment shader
    glGetShaderiv(fs, GL_COMPILE_STATUS, &result);
    glGetShaderiv(fs, GL_INFO_LOG_LENGTH, &infoLogLength);
    std::vector<char> fragmentShaderErrorMessage(infoLogLength);
    fprintf(stdout, "%Shader Message: s\n", &fragmentShaderErrorMessage[0]);

    //return something else if failed? or maybe throw exeption... we'll see
    if(result == GL_TRUE)
    {
        FSstruct f;
        f.name = _fsPath;
        f.fragmentShader = fs;
        fragmentShaders->push_back(f);

        printf("successfully compiled fragment shader: %s\n\n", _fsPath.c_str());

        return (int)fs;
    }
    else
    {
        printf("failed to compile fragment shader %s\n\n", _fsPath.c_str());
    }
    return -1;
}

void ProgramGLSL::useProgram()
{
    glUseProgram(programID);
}
