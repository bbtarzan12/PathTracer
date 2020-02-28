#include "LightIntersectTester.h"

#include "AreaLight.h"
#include "PointLight.h"

LightIntersectTester::LightIntersectTester(const Ray& ray, const float& epsilon)
	: bIntersect(false), tHit(0), normal(0), ray(ray), epsilon(epsilon)
{
}

void LightIntersectTester::Visit(const PointLight& light)
{
	shape = nullptr;
	bIntersect = light.Intersect(ray, tHit, normal, epsilon);
}

void LightIntersectTester::Visit(const AreaLight& light)
{
	shape = light.GetShape();
	bIntersect = light.Intersect(ray, tHit, normal, epsilon);
}

bool LightIntersectTester::GetIntersectInformation(float& t, glm::vec3& n, std::shared_ptr<Shape>& s) const
{
	s = shape;
	t = tHit;
	n = normal;
	return bIntersect;
}
