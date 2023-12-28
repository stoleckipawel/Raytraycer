#include "Light.h"
#include "imgui.h"
#include "glm/gtc/type_ptr.hpp"

void Light::BuildUI(uint32_t id)
{
	ImGui::PushID(id);
	ImGui::DragFloat3("Direction", glm::value_ptr(Direction), 0.01f);
	ImGui::ColorEdit3("Color", glm::value_ptr(Color));
	ImGui::DragFloat("Intensity", &Intensity, 0.01f);
	ImGui::Separator();
	ImGui::PopID();
}


