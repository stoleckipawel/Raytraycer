#include "Material.h"
#include "imgui.h"
#include "glm/gtc/type_ptr.hpp"
#include "Walnut/Random.h"

void Material::BuildUI(uint32_t id)
{
	ImGui::PushID(id);
	ImGui::ColorEdit3("Albedo", glm::value_ptr(Albedo));
	ImGui::ColorEdit3("Emmisive", glm::value_ptr(Emmisive));
	ImGui::DragFloat("Specular", &Specular, 0.5f, 0.0f, 1.0f);
	ImGui::DragFloat("Roughness", &Roughness, 0.01f, 0.0f, 1.0f);
	//ImGui::DragFloat("Opacity", &Opacity, 0.01f, 0.0f, 1.0f);
	//ImGui::DragFloat("IOR", &IOR, 0.01f);
	ImGui::Separator();
	ImGui::PopID();
}

glm::vec3 Material::Resolve(Ray& ray, Trace& trace) const
{
	float NoL = glm::clamp(glm::dot(-ray.Direction, trace.WorldNormal), 0.0f, 1.0f);
	ray.Transmission *= NoL;

	//Account of self light emmiting objects
	glm::vec3 incomingLight = Emmisive * ray.Transmission;

	//Compute next bounce Direction

	//Specular
	float fresnel = Utils::FresnelSchlick(Specular, AIR_IOR, 1.0f, ray.Direction, trace.WorldNormal);
	bool isSpecularBounce = fresnel >= Walnut::Random::Float();
	glm::vec3 specularDir = glm::reflect(ray.Direction, trace.WorldNormal);

	//Diffuse Dir
	glm::vec3 diffuseDir = Utils::RandomHemisphereDir(trace.WorldNormal);
	
	//Roughness integration
	float specularConeWidth = isSpecularBounce * (1.0 - Roughness);//the higher roughness the wider specular lobe
	ray.Direction = Utils::Lerp(diffuseDir, specularDir, specularConeWidth);

	//Apply Ray Bias
	ray.Origin = trace.WorldPosition + trace.WorldNormal * EPSILON;

	//Reduce Ray Transsmission
	glm::vec3 hitTranssmission = isSpecularBounce ? glm::vec3(fresnel, fresnel, fresnel) : Albedo;
	ray.Transmission *= hitTranssmission;
	return incomingLight;
}
