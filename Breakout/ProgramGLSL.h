#ifndef _WIN32

#ifndef _PROGRAMGLSL_H_
#define _PROGRAMGLSL_H_

#include <string>
#include <vector>
#include <GL/glew.h>
#include <GL/glfw.h>

struct VSstruct
{
	GLuint vertexShader;
	std::string name;
};
struct GSstruct
{
	GLuint geometryShader;
	std::string name;
};
struct FSstruct
{
	GLuint fragmentShader;
	std::string name;
};
class ProgramGLSL
{
    public:
        ProgramGLSL(std::string name, std::string vsPath, std::string gsPath, std::string fsPath);
        ~ProgramGLSL();

        std::string getName() { return name; };

        static void cleanup();

        void useProgram();
        GLuint getProgramID() { return programID; };

        // TODO: static void Cleanup(); to remove static lists

    protected:
    private:
        // These are not necessary. Only here to make it similar to HLSL-version
        static std::vector<VSstruct> *vertexShaders;
        static std::vector<GSstruct> *geometryShaders;
        static std::vector<FSstruct> *fragmentShaders;

        int insertVertexShader(std::string vsPath);
        int insertGeometryShader(std::string gsPath);
        int insertFragmentShader(std::string fsShader);

        int vertexShaderIndex;
        int geometryShaderIndex;
        int fragmentShaderIndex;

        int programID;

        std::string name;
};

#endif // ! _PROGRAMGLSL_H_

#endif // ! _WIN32
