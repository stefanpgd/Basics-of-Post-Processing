#include "Engine/Camera.h"
#include "Engine/Utilities.h"
#include "Graphics/Renderer.h"

#include <glm/gtc/matrix_transform.hpp>

Camera::Camera(const glm::vec3& position, const glm::vec3& front, const glm::vec3& up) :
	Position(position), Front(front), Up(up)
{
	viewMatrix = glm::lookAt(position, position + front, up);
	projectionMatrix = glm::perspective(glm::radians(FOV), (float)Renderer::GetWindowWidth() / (float)Renderer::GetWindowHeight(), nearClip, farClip);
}

void Camera::Update(float deltaTime)
{
	glm::vec3 position = Position;

	if(screenshakeTimer > 0.0f)
	{
		screenshakeTimer -= deltaTime;
		glm::vec3 shake = glm::vec3(
			RandomInRange(-screenshakeStrength, screenshakeStrength),
			RandomInRange(-screenshakeStrength, screenshakeStrength),
			RandomInRange(-screenshakeStrength, screenshakeStrength));

		shake *= shakeDirection;
		position += shake;
	}

	viewMatrix = glm::lookAt(position, position + Front, Up);
	projectionMatrix = glm::perspective(glm::radians(FOV), (float)Renderer::GetWindowWidth() / (float)Renderer::GetWindowHeight(), nearClip, farClip);
	viewProjectionMatrix = projectionMatrix * viewMatrix;
}

glm::mat4& Camera::GetViewProjectionMatrix()
{
	return viewProjectionMatrix;
}

float Camera::GetNearClip()
{
	return nearClip;
}

float Camera::GetFarClip()
{
	return farClip;
}

void Camera::ApplyScreenshake(float duration, float magnitude, const glm::vec3& direction)
{
	screenshakeTimer = duration;
	screenshakeStrength = magnitude;
	shakeDirection = direction;
}