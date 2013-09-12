
class GraphicsInterface
{
public:
	GraphicsInterface(){};
	virtual void init() = 0;
	virtual void cleanUp() = 0;
	virtual void compileShader() = 0;
	virtual void presentSwapChain() = 0;
};