#include "EnvironmentLight.h"
#include "glm/gtc/type_ptr.hpp"

void EnvironmentLight::BuildUI(uint32_t id)
{
	ImGui::PushID(id);
	ImGui::ColorEdit3("Sky Zenith Color", glm::value_ptr(SkyZenithColor));
	ImGui::ColorEdit3("Sky Horizon Color", glm::value_ptr(SkyHorizonColor));
	ImGui::DragFloat("Sky Intensity", &SkyIntensity, 0.01f, 0.0f, 10.0f);
	ImGui::Separator();
	ImGui::PopID();
}

glm::vec3 EnvironmentLight::SampleEnvironment(const Ray& ray, std::vector<const DirectionalLight*> directionalLights) const
{
	//#To do Environmental mapping

	//Sky Base
	glm::vec3 skyZenith = SkyZenithColor * SkyIntensity;
	glm::vec3 skyHorizon = SkyHorizonColor * SkyIntensity * 0.66f;
	float skyGradient = glm::sqrt(glm::max(0.0f, ray.Direction.y));
	glm::vec3 sky = skyHorizon * (1.0f - skyGradient) + skyZenith * skyGradient;

	glm::vec3 ground = groundAlbedo;
	for (int i = 0; i < directionalLights.size(); i++)
	{
		const DirectionalLight* directionalLight = directionalLights[i];

		//Compute ground sun shading assuming it's point up and is only lit with directional light
		//ground += directionalLight->CalculateShading(glm::vec3(0.0f, 1.0f, 0.0f), groundAlbedo);

		//Compute sun on the sky assuming there is no atmosphere right now
		float SunSize = 600.0f;
		float SunShape = glm::pow(directionalLight->CalculateDiffuseTerm(glm::normalize(ray.Direction)), SunSize);
		sky += SunShape * directionalLight->Color * directionalLight->Intensity;
	}

	float sky_opacity = glm::smoothstep(-0.01f, 0.00f, ray.Direction.y);
	//Mix sky with ground
	return ground * (1.0f - sky_opacity) + sky * sky_opacity;//lerp
}