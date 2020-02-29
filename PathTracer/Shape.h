#pragma once
#include <glm/glm.hpp>
#include <tuple>

class Ray;
struct IntersectInfo;

class Shape
{
public:
	Shape() : model(1.0f) {}
	virtual ~Shape() = default;

	virtual bool Intersect(const Ray& ray, float& tHit, glm::vec3& normal, float rayEpsilon) const = 0;
	virtual float GetArea() const = 0;
	virtual float GetPDF(const glm::vec3& point, const glm::vec3& wi) const = 0;
	virtual std::tuple<glm::vec3, glm::vec3> GetRandomPointOnSurface() const = 0;
	
	virtual void InitOpenGL() = 0;
	virtual void DrawOpenGL() = 0;
	virtual void ClearOpenGL() = 0;

	glm::mat4 model;

protected:

	// http://www.pbr-book.org/3ed-2018/Monte_Carlo_Integration/2D_Sampling_with_Multidimensional_Transformations.html#UniformConePdf
	float UniformConePDF(const float cosThetaMax) const;
};