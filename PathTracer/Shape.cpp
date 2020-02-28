#include "Shape.h"

#include <glm/gtc/constants.inl>

float Shape::UniformConePDF(const float cosThetaMax) const
{
	return 1.0f / (glm::two_pi<float>() * (1.0f - cosThetaMax));
}
