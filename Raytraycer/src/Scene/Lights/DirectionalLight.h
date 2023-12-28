#pragma once
#include "glm/glm.hpp"
#include "Light.h"

class DirectionalLight : public Light
{
public:
	virtual float CalculateDiffuseTerm(glm::vec3 Normal) const override;
	virtual glm::vec3 CalculateShading(glm::vec3 normal, glm::vec3 albedo) const override;
private:
};
