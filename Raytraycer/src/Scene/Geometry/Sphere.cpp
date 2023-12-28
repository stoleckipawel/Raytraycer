#include "Sphere.h"
#include "imgui.h"
#include "glm/gtc/type_ptr.hpp"

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
	//Ray sphere intersection
	//bx^2 +by^2)t^2 + (2axbx + 2ayby)t + (ax^2 + ay^2 - r^2) = 0
	//a = ray origin
	//b = ray direction
	//r = sphere radius

	float radiusSquared = Radius * Radius;
	glm::vec3 origin = ray.Origin - Position;//optimization for sphere placement

	float a = glm::dot(ray.Direction, ray.Direction);
	float b = glm::dot(origin, ray.Direction) * 2.0f;
	float c = glm::dot(origin, origin) - radiusSquared;

	//Quadriatic formula descriminant
	// x1 = b^2 - 4ac
	float discriminant = b * b - (4.0f * a * c);
	if (discriminant > 0.0f)
	{
		float hitDistance = (-b - glm::sqrt(discriminant)) / (2.0f * a);//closest
		//float t1 = (-b + glm::sqrt(discriminant)) / (2 * a);//not needed now
		return Trace().Hit(hitDistance, Guid);//to fix!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
	}
	else
	{
		return Trace().Miss();
	}
}

Trace Sphere::ResolveTrace(Trace& trace, const Ray& ray) const
{
	glm::vec3 origin = ray.Origin - Position;//optimization for sphere placement
	//ray origin + direction * distance = hit position
	trace.WorldPosition = origin + ray.Direction * trace.HitDistance;
	trace.WorldNormal = glm::normalize(trace.WorldPosition);//assumes that each sphere is in the center of the world
	//cancles optimization influence
	trace.WorldPosition += Position;
	return trace;
}

