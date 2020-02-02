#include "Random.h"

uint32_t randomSeed = 1;

uint32_t PathTracing::_random()
{
	uint32_t t = time(nullptr);
	randomSeed ^= t << 15;
	randomSeed ^= randomSeed << 13;
	randomSeed ^= randomSeed >> 17;
	randomSeed ^= randomSeed << 5;
	return randomSeed;
}

float PathTracing::RandomFloat()
{
	return (float)_random() / 0xffffffff;
}

glm::vec2 PathTracing::ConcentricSampleDisk(const glm::vec2& point)
{
	// [-1, 1]로 사상
	glm::vec2 offset = 2.0f * point - glm::vec2(1.0f, 1.0f);

	if (offset.x == 0 && offset.y == 0)
		return glm::vec2(0, 0);

	// r, theta로 사상
	float theta, r;

	if (glm::abs(offset.x) > glm::abs(offset.y))
	{
		r = offset.x;
		theta = glm::quarter_pi<float>() * (offset.y / offset.x);
	}
	else
	{
		r = offset.y;
		theta = glm::half_pi<float>() - glm::quarter_pi<float>() * (offset.x / offset.y);
	}

	return r * glm::vec2(glm::cos(theta), glm::sin(theta));
}

glm::vec3 PathTracing::UniformSampleHemisphere(const float r1, const float r2)
{
	float z = r1;
	float r = glm::sqrt(glm::max(0.0f, 1.0f - z * z));
	float phi = glm::two_pi<float>() * r2;
	float x = r * glm::cos(phi);
	float y = r * glm::sin(phi);

	return glm::vec3(x, y, z);
}
