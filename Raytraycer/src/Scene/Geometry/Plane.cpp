#include "Plane.h"

void Plane::BuildUI(uint32_t id)
{
	ImGui::PushID(id);
	ImGui::DragFloat3("Position", glm::value_ptr(Position), 0.01f);
	ImGui::DragFloat3("Scale", glm::value_ptr(Scale), 0.01f);
	ImGui::DragFloat3("Rotation", glm::value_ptr(Rotation), 0.01f);
	ImGui::Separator();
	ImGui::PopID();
}

Trace Plane::TraceRay(const Ray& ray) const
{
	return Trace();
}

Trace Plane::ResolveTracePayload(Trace& payload, const Ray& ray) const
{
	return Trace();
}
