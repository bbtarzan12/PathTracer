#include "Bxdf.h"

#include "Random.h"

glm::vec3 Bxdf::SampleF(const glm::vec3& wo, glm::vec3& wi, float& pdf) const
{
	const auto[r1, r2] = PathTracing::RandomFloat2();
	wi = PathTracing::UniformSampleHemisphere(r1, r2);
	pdf = PDF(wo, wi);

	return CalculateF(wo, wi);
}

float Bxdf::PDF(const glm::vec3& wo, const glm::vec3& wi) const
{
	return PathTracing::UniformHemispherePDF;
}

float Bxdf::CosTheta(const glm::vec3& v)
{
	// 0,0,1 Dot v = v.z
	return v.z;
}

float Bxdf::SinTheta(const glm::vec3& v)
{
	return glm::max(0.0f, glm::sqrt(1.0f - v.z * v.z));
}

float Bxdf::CosPhi(const glm::vec3& v)
{
	const float r = SinTheta(v);
	if (r == 0.0f)
		return 1.0f;

	return glm::clamp(v.x / r, -1.0f, 1.0f);
}

float Bxdf::SinPhi(const glm::vec3& v)
{
	const float r = CosTheta(v);
	if (r == 0.0f)
		return 1.0f;

	return glm::clamp(v.y / r, -1.0f, 1.0f);
}
