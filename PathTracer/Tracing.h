#pragma once

#include <vector>
#include <memory>

struct IntersectInfo;
class Ray;
class SceneObject;
class Light;

namespace PathTracing
{	
	bool TraceRay(const Ray& ray, IntersectInfo& info, const float epsilon, const std::vector<std::shared_ptr<SceneObject>>& objects, const std::vector<std::shared_ptr<Light>>& lights);
}
