#pragma once

#include "Shape.h"
#include <glm/glm.hpp>
#include <GL/glew.h>
#include <glm/common.hpp>
#include <glm/common.hpp>
#include <glm/common.hpp>
#include <glm/common.hpp>

class Sphere : public Shape
{
public:
	Sphere(const glm::vec3& center, const float radius);

	virtual bool Intersect(const Ray& ray, float& tHit, glm::vec3& normal, float rayEpsilon) const override;
	virtual float GetArea() const override;
	virtual float GetPDF(const glm::vec3& point, const glm::vec3& lightDir) const override;
	virtual std::tuple<glm::vec3, glm::vec3> GetRandomPointOnSurface() const override;
	
	virtual void InitOpenGL() override;
	virtual void DrawOpenGL() override;
	virtual void ClearOpenGL() override;



private:
	bool SolveQuadratic(const float a, const float b, const float c, float& t0, float& t1) const;

public:
	glm::vec3 center;
	float radius;

	bool isInited;
	GLuint VAO, VBO, EBO;
	int numsToDraw;
};
