#include "Shape.h"

#include <glm/gtc/constants.inl>

Shape::~Shape()
{
	ClearOpenGL();
}

float Shape::UniformConePDF(const float cosThetaMax) const
{
	return 1.0f / (glm::two_pi<float>() * (1.0f - cosThetaMax));
}
