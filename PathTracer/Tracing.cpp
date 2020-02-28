#include "Tracing.h"
#include "AreaLight.h"
#include "Common.h"
#include "glm/glm.hpp"
#include "IntersectInfo.h"
#include "Shape.h"
#include "LightIntersectTester.h"

#include <memory>


bool PathTracing::TraceRay(const Ray& ray, IntersectInfo& info, const float epsilon, const std::vector<std::shared_ptr<Shape>>& shapes, const std::vector<std::shared_ptr<Light>>& lights)
{
	info.t = PathTracing::INFINITE<float>;
	float tHit = 0;
	glm::vec3 normal(0);
	for (auto& shape : shapes)
	{
		if (shape->Intersect(ray, tHit, normal, epsilon) && tHit < info.t)
		{
			info.t = tHit;
			info.shape = shape;
			info.normal = normal;
		}
	}

	for (auto& light : lights)
	{
		LightIntersectTester lightIntersectTester(ray, epsilon);
		light->Accept(lightIntersectTester);
		std::shared_ptr<Shape> shape;
		if(lightIntersectTester.GetIntersectInformation(tHit, normal, shape) && tHit < info.t)
		{
			info.t = tHit;
			info.shape = shape;
			info.normal = normal;
		}
	}

	return info.shape.lock() != nullptr;
}
