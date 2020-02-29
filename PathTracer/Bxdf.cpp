#include "Bxdf.h"

#include "Random.h"

glm::vec3 Bxdf::SampleF(const glm::vec3& wo, glm::vec3& wi, float& pdf)
{
	const auto[r1, r2] = PathTracing::RandomFloat2();
	wi = PathTracing::UniformSampleHemisphere(r1, r2);
	pdf = PDF(wo, wi);

	return CalculateF(wo, wi);
}

float Bxdf::PDF(const glm::vec3& wo, const glm::vec3& wi)
{
	return PathTracing::UniformHemispherePDF;
}
