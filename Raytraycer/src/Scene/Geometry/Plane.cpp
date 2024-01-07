#include "Plane.h"

Plane::Plane()
{
	Rotation = glm::vec3(0.0f, 1.0f, 0.0f);
}

void Plane::BuildUI(uint32_t id)
{
	ImGui::PushID(id);
	ImGui::DragFloat3("Position", glm::value_ptr(Position), 0.01f);
	ImGui::DragFloat3("Rotation", glm::value_ptr(Rotation), 0.01f);
	ImGui::Separator();
	ImGui::PopID();
}

Trace Plane::Intersect(const Ray& ray) const
{
	Trace trace;
	float denominator = glm::dot(ray.Direction, Rotation);
	if (denominator > 0.0f);//avoid case when directional & normal is completely perpendicular
	{
		glm::vec3 p0l0 = Position - ray.Origin;
		float t = (glm::dot(p0l0, Rotation)) / denominator;
		if (t >= 0)
		{
			trace.Hit(t, Guid);
			trace.WorldNormal = Rotation;
			return trace;
		}
		
	}

	return trace.Miss();
}

Trace Plane::ResolveTracePayload(Trace& trace, const Ray& ray) const
{
	trace.WorldPosition = ray.Origin + ray.Direction * trace.HitDistance;
	return trace;
}
