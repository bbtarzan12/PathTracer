#include "Random.h"

#include <ctime>

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
	return static_cast<float>(_random()) / 0xffffffff;
}

std::tuple<float, float> PathTracing::RandomFloat2()
{
	return std::make_tuple(RandomFloat(), RandomFloat());
}

glm::vec2 PathTracing::ConcentricSampleDisk(const glm::vec2& point)
{
	// [-1, 1]로 사상
	const glm::vec2 offset = 2.0f * point - glm::vec2(1.0f, 1.0f);

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

glm::vec3 PathTracing::UniformSampleSphere(const float r1, const float r2)
{
	float z = 2 * r1 - 1;
	float r = glm::sqrt(glm::max(0.0f, 1.0f - z * z));
	float phi = glm::two_pi<float>() * r2;
	float x = r * glm::cos(phi);
	float y = r * glm::sin(phi);

	return glm::vec3(x, y, z);
}

glm::vec3 PathTracing::UniformSampleCone(const float r1, const float r2, const float cosThetaMax)
{
	float cosTheta = (1.0f - r1) + r1 * cosThetaMax;
	float sinTheta = std::sqrt(1.0f - cosTheta * cosTheta);
	float phi = r2 * glm::two_pi<float>();
	return glm::vec3(glm::cos(phi) * sinTheta, glm::sin(phi) * sinTheta, cosTheta);
}

glm::vec3 PathTracing::UniformSampleCone(const float r1, const float r2, const float cosThetaMax, const glm::vec3& x, const glm::vec3& y, const glm::vec3& z)
{
	float cosTheta = glm::mix(cosThetaMax, 1.f, r1);
	float sinTheta = sqrtf(1.f - cosTheta * cosTheta);
	float phi = r2 * glm::two_pi<float>();
	return glm::cos(phi) * sinTheta * x + glm::sin(phi) * sinTheta * y + cosTheta * z;
}
