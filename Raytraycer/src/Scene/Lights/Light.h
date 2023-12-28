#pragma once
#include "glm/glm.hpp"

class Light
{
public:
	virtual void BuildUI(uint32_t id);
	virtual float CalculateDiffuseTerm(glm::vec3 Normal) const { return 0.0f; };
	virtual glm::vec3 CalculateShading(glm::vec3 normal, glm::vec3 albedo) const { return glm::vec3(0.0f, 0.0f, 0.0f); };
public:
	glm::vec3 Direction = glm::vec3(0.0f, -1.0f, 0.0f);
	glm::vec3 Color = glm::vec3(1.0f, 1.0f, 1.0f);
	float Intensity = 1.0f;
};
