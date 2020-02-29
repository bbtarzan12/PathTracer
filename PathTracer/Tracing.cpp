#include "Tracing.h"
#include "AreaLight.h"
#include "Common.h"
#include "glm/glm.hpp"
#include "IntersectInfo.h"
#include "Shape.h"
#include "LightIntersectTester.h"

#include "Math.h"
#include "SceneObject.h"
#include <memory>


bool PathTracing::TraceRay(const Ray& ray, IntersectInfo& info, const float epsilon, const std::vector<std::shared_ptr<SceneObject>>& objects, const std::vector<std::shared_ptr<Light>>& lights)
{
	info.t = PathTracing::INFINITE<float>;
	info.emit = glm::vec3(0);
	float tHit = 0;
	glm::vec3 normal(0);
	for (auto& object : objects)
	{
		const auto& shape = object->GetShape().lock();
		const auto& material = object->GetMaterial().lock();

		if(shape == nullptr || material == nullptr)
			continue;

		if (shape->Intersect(ray, tHit, normal, epsilon) && tHit < info.t)
		{
			info.t = tHit;
			info.shape = shape;
			info.normal = normal;
			info.material = material;
		}
	}

	for (auto& light : lights)
	{
		LightIntersectTester lightIntersectTester(ray, epsilon);
		light->Accept(lightIntersectTester);
		std::shared_ptr<Shape> shape;
		glm::vec3 emit;
		if (lightIntersectTester.GetIntersectInformation(tHit, normal, emit, shape) && tHit < info.t)
		{
			info.t = tHit;
			info.shape = shape;
			info.normal = normal;
			info.emit = emit;
		}
	}

	const bool bHit = info.shape.lock() != nullptr;

	if (!bHit)
	{
		return false;
	}

	auto[tangent, biTangent] = BuildTangentSpace(info.normal);

	info.tangent = tangent;
	info.biTangent = biTangent;

	return true;
}
