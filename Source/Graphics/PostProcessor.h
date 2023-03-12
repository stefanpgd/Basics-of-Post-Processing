#pragma once
#include <glm/glm.hpp>

class Framebuffer;
class ShaderProgram;
class ScreenQuad;

class PostProcessor
{
public:
	PostProcessor(unsigned int windowWidth, unsigned int windowHeight);

	void PostProcess(Framebuffer* sceneBuffer);
	void DebugDrawImGui();

private:
	// Screen, HDR, Exposure //
	ScreenQuad* screenQuad;
	ShaderProgram* screenShader;
};