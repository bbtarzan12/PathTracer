#pragma once
#include <glm/vec3.hpp>
#include <memory>
#include <vector>

#include "LightVisitor.h"

struct IntersectInfo;
class Shape;
class Light;
class Ray;

class LightIntersectTester : public LightVisitor
{
	
public:
	LightIntersectTester(const Ray& ray, const float& epsilon);
	virtual void Visit(const PointLight& light) override;
	virtual void Visit(const AreaLight& light) override;
	bool GetIntersectInformation(float& t, glm::vec3& n, std::shared_ptr<Shape>& s) const;

private:
	bool bIntersect;
	float tHit;
	glm::vec3 normal;
	std::shared_ptr<Shape> shape;
	
	const Ray& ray;
	const float& epsilon;
};
