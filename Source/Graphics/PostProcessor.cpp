#include "Graphics/PostProcessor.h"
#include "Graphics/ShaderProgram.h"
#include "Graphics/ScreenQuad.h"
#include "Graphics/Framebuffer.h"

#include <glad/glad.h>
#include <imgui.h>

PostProcessor::PostProcessor(unsigned int windowWidth, unsigned int windowHeight)
{
	screenQuad = new ScreenQuad();
	screenShader = new ShaderProgram("screen.vert", "screen.frag");

	// Apply constant uniform data // 
	screenShader->Bind();
	screenShader->SetInt("sceneTexture", 0);
}

void PostProcessor::PostProcess(Framebuffer* sceneBuffer)
{
	// Enable default buffer // 
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
	glClear(GL_DEPTH_BUFFER_BIT);

	// Final post-processing pass (Chromatic Aberration, HDR, Gamma Correction) // 
	screenShader->Bind();
	sceneBuffer->BindTexture(0, 0);

	screenQuad->Render();
}

void PostProcessor::DebugDrawImGui()
{
	ImGui::Begin("Screen Settings");
	ImGui::End();
}