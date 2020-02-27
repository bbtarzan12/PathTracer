#pragma once

#include "Shape.h"
#include <glm/glm.hpp>
#include <GL/glew.h>

class Sphere : public Shape
{
public:
	Sphere(const glm::vec3& center, float radius, const glm::vec3& color = glm::vec3(1,1,1), const glm::vec3& emit = glm::vec3(0,0,0));

	virtual bool Intersect(const Ray& ray, float& tHit, glm::vec3& normal, float rayEpsilon) const override;
	virtual float GetArea() const override;
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
