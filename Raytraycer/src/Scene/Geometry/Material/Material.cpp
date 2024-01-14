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
	//Account of self light emmiting objects
	glm::vec3 incomingLight = Emmisive * ray.Transmission;

	//Compute next bounce Direction
	float fresnel = Utils::FresnelSchlick(Specular, AIR_IOR, 1.0f, ray.Direction, trace.WorldNormal);
	bool isSpecularBounce = fresnel >= Walnut::Random::Float();//assumes temporal accumulation

	glm::vec3 hitTranssmission = isSpecularBounce ? glm::vec3(fresnel, fresnel, fresnel) : Albedo;
	ray.Transmission *= hitTranssmission;

	glm::vec3 diffuseDir = Utils::RandomHemisphereDir(trace.WorldNormal);

	glm::vec3 reflectedDir = glm::reflect(ray.Direction, trace.WorldNormal);
	float reflectedDirOpacity = isSpecularBounce * (1.0 - Roughness);//Roughnes & Specular Integration
	ray.Direction = diffuseDir * (1.0f - reflectedDirOpacity) + reflectedDir * reflectedDirOpacity;//lerp

	ray.Origin = trace.WorldPosition + trace.WorldNormal * EPSILON;//bias

	return incomingLight;
}
