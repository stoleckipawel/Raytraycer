#include "Plane.h"

void Plane::BuildUI(uint32_t id)
{
	ImGui::PushID(id);
	ImGui::DragFloat3("Position", glm::value_ptr(Position), 0.01f);
	ImGui::Separator();
	ImGui::PopID();
}

Trace Plane::Intersect(const Ray& ray) const
{
	glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);
	glm::vec3 normal = up;// rotation 

	float denominator = glm::dot(ray.Direction, normal);
	if (denominator > std::numeric_limits<float>::min());
	{
		glm::vec3 p0l0 = Position - ray.Origin;
		float t = (glm::dot(p0l0, normal)) / denominator;
		if (t >= 0)
		{
			return Trace().Hit(t, Guid);
		}
		
	}

	return Trace().Miss();
}

Trace Plane::ResolveTracePayload(Trace& trace, const Ray& ray) const
{
	trace.WorldPosition = ray.Origin + ray.Direction * trace.HitDistance;

	glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);
	trace.WorldNormal = up;// rotation 
	return trace;
}
