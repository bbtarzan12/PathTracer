#include "Ray.h"

Ray::Ray()
	: origin(0, 0, 0), direction(0, 0, 0), mint(0), maxt(PathTracing::INFINITE<float>), depth(0)
{

}

Ray::Ray(const glm::vec3& origin, const glm::vec3& direction, float start, float end /*= INFINITE*/, int d /*= 0*/)
	: origin(origin), direction(direction), mint(start), maxt(end), depth(d)
{

}

Ray::Ray(const glm::vec3& origin, const glm::vec3& direction, const Ray& parent, float start, float end /*= INFINITE*/, int d /*= 0*/)
	: origin(origin), direction(direction), mint(start), maxt(end), depth(parent.depth + 1)
{

}
