#pragma once
#include "glm/glm.hpp"
#include "../../Ray.h"
#include "../../../Utils/Trace.h"
#include "../../../Utils/Utils.h"

class Material
{
public:
	void BuildUI(uint32_t id);
	glm::vec3 Resolve(Ray& ray, Trace& trace) const;
public:
	glm::vec3 Albedo = glm::vec3(1.0f, 1.0f, 1.0f);
	glm::vec3 Emmisive = glm::vec3(0.0f, 0.0f, 0.0f);
	float Specular = 0.3f;
	float Roughness = 0.07f;
	float Opacity = 1.0f;
	float IOR = 1.0f;
private:
	const float AIR_IOR = 1.0003f;
	const float EPSILON = 1.0f / 255.0f;
};

