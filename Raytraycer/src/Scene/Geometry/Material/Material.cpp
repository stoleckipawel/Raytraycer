#include "Material.h"
#include "imgui.h"
#include "glm/gtc/type_ptr.hpp"

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
