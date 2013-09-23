#ifndef _WIN32

#ifndef _TECHNIQUEGLSL_H_
#define _TECHNIQUEGLSL_H_

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
class TechniqueGLSL
{
    public:
        TechniqueGLSL(std::string name, std::string vsPath, std::string gsPath, std::string fsPath);
        ~TechniqueGLSL();

        std::string getName() { return name; };

        static void cleanup();

        void useTechnique();

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

        int techniqueID;

        std::string name;
};

#endif // _TECHNIQUEGLSL_H_

#endif // ! _WIN32
