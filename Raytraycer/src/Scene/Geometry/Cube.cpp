#include "Cube.h"

void Box::BuildUI(uint32_t id)
{
	ImGui::PushID(id);
	ImGui::DragFloat3("Position", glm::value_ptr(Position), 0.01f);
	ImGui::DragFloat3("Scale", glm::value_ptr(Scale), 0.01f);
	ImGui::Separator();
	ImGui::PopID();
}

glm::vec3 Box::GetExtentMin() const
{
	return Position - Scale * 0.5f;
}

glm::vec3 Box::GetExtentMax() const 
{
	return Position +- Scale * 0.5f;
}

Trace Box::Intersect(const Ray& ray) const
{
	glm::vec3 extentMin = GetExtentMin();
	glm::vec3 extentMax = GetExtentMax();
	return Trace();
}

Trace Box::ResolveTracePayload(Trace& payload, const Ray& ray) const
{
	return Trace();
}
