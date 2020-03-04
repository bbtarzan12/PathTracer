#include "Ray.h"

Ray::Ray()
	: origin(0, 0, 0), direction(0, 0, 0), depth(0), mint(0), maxt(PathTracing::INFINITE<float>)
{

}

Ray::Ray(const glm::vec3& origin, const glm::vec3& direction, float start, float end /*= INFINITE*/, int d /*= 0*/)
	: origin(origin), direction(direction), depth(d), mint(start), maxt(end)
{

}

Ray::Ray(const glm::vec3& origin, const glm::vec3& direction, const Ray& parent, float start, float end /*= INFINITE*/)
	: origin(origin), direction(direction), depth(parent.depth + 1), mint(start), maxt(end)
{

}
