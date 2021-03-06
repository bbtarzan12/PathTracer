#pragma once

#include "Shape.h"
#include <glm/glm.hpp>
#include <GL/glew.h>

class Sphere : public Shape
{
public:
	Sphere(const glm::vec3& center, const float radius);

	virtual bool Intersect(const Ray& ray, float& tHit, glm::vec3& normal, float rayEpsilon) const override;
	virtual float GetArea() const override;

	// 영역이 아닌 점 point에 대한 입체각으로 정의
	virtual float GetPDF(const glm::vec3& point, const glm::vec3& lightDir) const override;
	virtual std::tuple<glm::vec3, glm::vec3> Sample() const override;
	virtual std::tuple<glm::vec3, glm::vec3> Sample(const glm::vec3& point) const override;
	
	virtual void InitOpenGL() override;
	virtual void DrawOpenGL() const override;
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
