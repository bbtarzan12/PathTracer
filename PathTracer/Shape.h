#pragma once
#include <glm/glm.hpp>
#include <tuple>

class Ray;
struct IntersectInfo;

class Shape
{
public:
	Shape(const glm::vec3& color, const glm::vec3& emit) : model(1.0f), color(color), emit(emit) {}
	virtual ~Shape() = default;

	virtual bool Intersect(const Ray& ray, float& tHit, glm::vec3& normal, float rayEpsilon) const = 0;
	virtual float GetArea() const = 0;
	virtual std::tuple<glm::vec3, glm::vec3> GetRandomPointOnSurface() const = 0;
	
	virtual void InitOpenGL() = 0;
	virtual void DrawOpenGL() = 0;
	virtual void ClearOpenGL() = 0;

	glm::mat4 model;
	glm::vec3 color;
	glm::vec3 emit;
};