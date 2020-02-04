#pragma once

#include "Camera.h"

class PinHoleCamera : public Camera
{
public:
	PinHoleCamera(const glm::uvec2& size, float fov, float focalLength, float aperture, const glm::vec2& clippingPlanes);

	virtual Ray GenerateRay(int x, int y) const override;
	virtual void UpdateScreen(int width, int height) override;
	virtual void UpdateCamera(float deltaTime, const glm::vec2& keyboard = glm::vec2(0, 0), const glm::vec2& mouse = glm::vec2(0, 0), bool bForce = false) override;
	virtual void HandleInput(float deltaTime, int key) override;

public:
	glm::vec3 forward, up, right;

	float fov;
	float aspectRatio;
	float focalLength;
	float aperture;
	glm::vec2 clippingPlanes;
	glm::uvec2 size;

	float theta, phi;
	float moveSpeed, mouseSpeed;
};