#pragma once
#include <glm/glm.hpp>
#include <tuple>

class Ray;
struct IntersectInfo;

class Shape
{
public:
	Shape() : model(1.0f) {}
	virtual ~Shape();

	virtual bool Intersect(const Ray& ray, float& tHit, glm::vec3& normal, float rayEpsilon) const = 0;
	virtual float GetArea() const = 0;

	virtual float GetPdf() const;
	virtual float GetPDF(const glm::vec3& point, const glm::vec3& wi) const;

	// point�� �־����� �ʾ��� ��
	virtual std::tuple<glm::vec3, glm::vec3> Sample() const = 0;

	// point�� �־����� ������ ����Ǵ� Sample�� ����.
	virtual std::tuple<glm::vec3, glm::vec3> Sample(const glm::vec3& point) const = 0;
	
	virtual void InitOpenGL() = 0;
	virtual void DrawOpenGL() const = 0;
	virtual void ClearOpenGL(){};

	glm::mat4 model;

protected:

	// http://www.pbr-book.org/3ed-2018/Monte_Carlo_Integration/2D_Sampling_with_Multidimensional_Transformations.html#UniformConePdf
	float UniformConePDF(const float cosThetaMax) const;
};