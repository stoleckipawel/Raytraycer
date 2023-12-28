#include "Sphere.h"

void Sphere::BuildUI(uint32_t id)
{
	ImGui::PushID(id);
	ImGui::DragFloat3("Position", glm::value_ptr(Position), 0.01f);
	ImGui::DragFloat("Radius", &Radius, 0.01f);
	ImGui::Separator();
	ImGui::PopID();
}

Trace Sphere::TraceRay(const Ray& ray) const
{
	//src: https://www.scratchapixel.com/lessons/3d-basic-rendering/minimal-ray-tracer-rendering-simple-shapes/ray-sphere-intersection.html
	glm::vec3 L = Position - ray.Origin;
	float Tca = glm::dot(L, ray.Direction);
	if (Tca < 0.0f)
		return Trace().Miss();//Tca is negative then early out since ray faces opposite direction

	float D = glm::sqrt(glm::dot(L, L) - glm::dot(Tca, Tca));
	if (D < 0.0f)//the ray misses the sphere, and there's no intersection (the ray overshoots the sphere)
		return Trace().Miss();

	float Thc = glm::sqrt((Radius * Radius) - (D * D));

	float closestT = Tca - Thc;
	//float furtherT = Tca + Thc;

	return Trace().Hit(closestT, Guid);
}

Trace Sphere::ResolveTracePayload(Trace& trace, const Ray& ray) const
{
	trace.WorldPosition = ray.Origin + ray.Direction * trace.HitDistance;
	trace.WorldNormal = glm::normalize(trace.WorldPosition - Position);
	return trace;
}

