#include "Material.h"
#include "IntersectInfo.h"
#include "Math.h"
#include "Random.h"

glm::vec3 Material::CalculateF(const IntersectInfo& info, const glm::vec3& worldWo, const glm::vec3& worldWi) const
{
	bitmask<BxdfType> mask = BxdfType::ALL;
	// 만약 worldWi와 worldWo가 같은 반구 내에 있다면 Reflection, 아니면 Transmission
	if (glm::dot(info.normal, worldWi) * glm::dot(info.normal, worldWo) > 0.0f)
	{
		mask &= ~BxdfType::TRANSMISSION;
	}
	else
	{
		mask &= ~BxdfType::REFLECTION;
	}

	const glm::vec3 wo = PathTracing::WorldToLocal(worldWo, info.normal, info.tangent, info.biTangent);
	const glm::vec3 wi = PathTracing::WorldToLocal(worldWi, info.normal, info.tangent, info.biTangent);
	
	glm::vec3 f(0.0f);

	for(const auto& bxdf : bxdfs)
	{
		if(bxdf->MatchType(mask))
			f += bxdf->CalculateF(wo, wi);
	}
	
	return f;
}

glm::vec3 Material::SampleF(const IntersectInfo& info, const glm::vec3& worldWo, glm::vec3& worldWi, float& pdf, bitmask<BxdfType>& type) const
{
	const Bxdf* selectedBxdf = nullptr;
	bitmask<BxdfType> mask = BxdfType::ALL;
	const int numBxdf = bxdfs.size();

	while(true)
	{
		// Todo : O(c) 알고리즘 찾아보기
		const int random = PathTracing::RandomInt(0, numBxdf - 1);
		const auto& bxdf = bxdfs[random];

		if (!bxdf->MatchType(mask))
		{
			continue;
		}

		selectedBxdf = bxdf.get();
		break;
	}

	if(selectedBxdf == nullptr)
	{
		pdf = 0.0f;
		type = BxdfType::REFLECTION;
		return glm::vec3(0.0f);
	}

	const glm::vec3 wo = PathTracing::WorldToLocal(worldWo, info.normal, info.tangent, info.biTangent);
	glm::vec3 wi(0.0f);
	glm::vec3 f = selectedBxdf->SampleF(wo, wi, pdf);
	worldWi = PathTracing::LocalToWorld(wi, info.normal, info.tangent, info.biTangent);

	if(pdf == 0.0f)
	{
		type = BxdfType::REFLECTION;
		return glm::vec3(0.0f);
	}

	int sameType = 1;
	for (const auto& bxdf : bxdfs)
	{
		if (selectedBxdf == bxdf.get() || !bxdf->MatchType(mask))
			continue;

		pdf += bxdf->PDF(wo, wi);
		sameType++;
	}
	pdf /= float(sameType);
	type = selectedBxdf->GetType();

	// 만약 worldWi와 worldWo가 같은 반구 내에 있다면 Reflection, 아니면 Transmission
	if (glm::dot(info.normal, worldWi) * glm::dot(info.normal, worldWo) > 0.0f)
	{
		mask &= ~BxdfType::TRANSMISSION;
	}
	else
	{
		mask &= ~BxdfType::REFLECTION;
	}

	// SampleF가 아닌 이유는 이미 wo, wi가 있기 때문에 바로 F를 구하면 더 빠르다
	for(const auto& bxdf : bxdfs)
	{
		if (selectedBxdf == bxdf.get() || !bxdf->MatchType(mask))
			continue;

		f += bxdf->CalculateF(wo, wi);
	}

	return f;
}
