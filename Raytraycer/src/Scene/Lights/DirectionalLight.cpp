#include "DirectionalLight.h"

float DirectionalLight::CalculateDiffuseTerm(glm::vec3 normal) const
{
	//Lambert
	//#To do: Disney
	glm::vec3 lightDir = glm::normalize(Direction);
	return glm::max(glm::dot(normal, lightDir), 0.0f);
}

glm::vec3 DirectionalLight::CalculateShading(glm::vec3 normal, glm::vec3 albedo) const
{
	float diffuseTerm = CalculateDiffuseTerm(normal);
	return diffuseTerm * Color * Intensity;
}
