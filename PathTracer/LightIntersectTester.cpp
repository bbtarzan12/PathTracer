#include "LightIntersectTester.h"

#include "AreaLight.h"
#include "PointLight.h"
#include "Ray.h"

LightIntersectTester::LightIntersectTester(const Ray& ray, const float& epsilon)
	: bIntersect(false), tHit(0), normal(0), emit(), shape(nullptr), ray(ray), epsilon(epsilon)
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
	emit = light.Emit();
}

bool LightIntersectTester::GetIntersectInformation(float& t, glm::vec3& n, glm::vec3& e, const Shape*& s) const
{
	s = shape;
	t = tHit;
	n = normal;
	e = emit;
	return bIntersect;
}
