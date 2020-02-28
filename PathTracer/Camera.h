#pragma once

#include <glm/glm.hpp>

class Ray;

class Camera
{
public:
	Camera() : position(0, 0, 0), proj(1.0f), view(1.0f), dirty(false) {};
	virtual ~Camera() = default;
	virtual Ray GenerateRay(int x, int y) const = 0;
	virtual void UpdateScreen(int width, int height) = 0;
	virtual void UpdateCamera(float deltaTime, const glm::vec2& keyboard = glm::vec2(0, 0), const glm::vec2& mouse = glm::vec2(0, 0), bool bForce = false) = 0;
	virtual void HandleInput(float deltaTime, int key) = 0;
	virtual void ResetDirty() { dirty = false; }

public:
	glm::vec3 position;
	glm::mat4 proj;
	glm::mat4 view;
	bool dirty;
};