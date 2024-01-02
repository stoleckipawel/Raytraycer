#pragma once
#include "glm/glm.hpp"
#include "imgui.h"
#include "../Ray.h"
#include "DirectionalLight.h"
#include <vector>
#include <memory>
#include "Light.h"

class EnvironmentLight
{
public:
	float SkyIntensity = 1.0f;
	glm::vec3 SkyZenithColor = glm::vec3(0.8f, 0.865f, 1.0f);
	glm::vec3 SkyHorizonColor = glm::vec3(0.7f, 1.0f, 0.99f);
public:
	void BuildUI(uint32_t id);
	glm::vec3 SampleEnvironment(const Ray& ray, std::vector<const DirectionalLight*> directionalLights) const;
private:
	glm::vec3 groundAlbedo = glm::vec3(0.31f, 0.31f, 0.31f);
};

