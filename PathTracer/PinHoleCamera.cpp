#include "PinHoleCamera.h"
#include "Ray.h"
#include "Random.h"
#include <glm/ext/matrix_transform.inl>
#include <glm/ext/matrix_clip_space.inl>
#include <iostream>
#include <GLFW/glfw3.h>

PinHoleCamera::PinHoleCamera(glm::uvec2 size, float fov, float focalLength, float aperture, glm::vec2 clippingPlanes)
	:
	up(0, 1, 0),
	forward(0, 0, 1),
	right(1, 0, 0),
	size(size),
	fov(fov),
	focalLength(focalLength),
	aperture(aperture),
	clippingPlanes(clippingPlanes),
	aspectRatio(size.x / (float)size.y),
	moveSpeed(10.0f),
	mouseSpeed(20.0f)
{
	UpdateCamera(0.01f, { 0, 0 }, { 0, 0 }, true);
}

Ray PinHoleCamera::GenerateRay(int x, int y) const
{
	// -0.5 ~ 0.5
	float jitter1 = PathTracing::RandomFloat() - 0.5f;
	float jitter2 = PathTracing::RandomFloat() - 0.5f;

	float top = glm::tan(glm::radians(fov * 0.5f));
	float bottom = -top;
	float left = bottom * aspectRatio;
	float right = -left;

	glm::vec3 wDir = glm::normalize(-forward);
	glm::vec3 uDir = glm::normalize(glm::cross(up, wDir));
	glm::vec3 vDir = glm::cross(wDir, -uDir);

	glm::vec2 imagePlaneUV
	{
		left + (right - left) * ((x + 0.5f + jitter1) / (float)size.x),
		bottom + (top - bottom) * ((y + 0.5f + jitter2) / (float)size.y)
	};

	glm::vec3 direction = glm::normalize(imagePlaneUV.x * uDir + imagePlaneUV.y * vDir - wDir);
	if (aperture > 0)
	{
		glm::vec3 pointOnImagePlane = position + direction * focalLength;

		float r1 = PathTracing::RandomFloat();
		float r2 = PathTracing::RandomFloat();
		glm::vec2 lensUV = PathTracing::ConcentricSampleDisk(glm::vec2(r1, r2));
		lensUV *= aperture;

		glm::vec3 aperturePoint = position + wDir * lensUV.x + uDir * lensUV.y;

		return Ray(aperturePoint, glm::normalize(pointOnImagePlane - aperturePoint), clippingPlanes.x, clippingPlanes.y);
	}
	else
	{
		return Ray(position, direction, clippingPlanes.x, clippingPlanes.y);
	}
}

void PinHoleCamera::UpdateScreen(int width, int height)
{
	size.x = width;
	size.y = height;
	aspectRatio = width / (float)height;
	dirty = true;
}

void PinHoleCamera::UpdateCamera(float deltaTime, glm::vec2 keyboard, glm::vec2 mouse, bool bForce)
{
	if (bForce == false && glm::length(keyboard) == 0 && glm::length(mouse) == 0)
		return;

	pitch += mouseSpeed * mouse.x * deltaTime;
	yaw += mouseSpeed * mouse.y * deltaTime;

	pitch = glm::clamp(pitch, -89.0f, 89.0f);
	yaw = glm::mod(yaw, 360.0f);

	position += (forward * keyboard.x + right * keyboard.y) * moveSpeed * deltaTime;
	forward.x = glm::cos(glm::radians(pitch)) * glm::sin(glm::radians(yaw));
	forward.y = glm::sin(glm::radians(pitch));
	forward.z = glm::cos(glm::radians(pitch)) *glm::cos(glm::radians(yaw));
	forward = glm::normalize(forward);
	right = glm::normalize(glm::cross(forward, glm::vec3(0, 1, 0)));
	up = glm::normalize(glm::cross(right, forward));

	view = glm::lookAt(position, position + forward, up);
	proj = glm::perspective(glm::radians(fov), aspectRatio, clippingPlanes.x, clippingPlanes.y);

	dirty = true;
}

void PinHoleCamera::HandleInput(float deltaTime, int key)
{
	if (key == GLFW_KEY_Z)
	{
		aperture += 0.1f;
	}

	if (key == GLFW_KEY_X)
	{
		aperture -= 0.1f;
	}

	if (key == GLFW_KEY_C)
	{
		focalLength += 0.1f;
	}

	if (key == GLFW_KEY_V)
	{
		focalLength -= 0.1f;
	}

	dirty = true;
}
