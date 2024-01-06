#include "Box.h"

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
	return Position + Scale * 0.5f;
}

Trace Box::Intersect(const Ray& ray) const
{
	glm::vec3 lb = GetExtentMin();
	glm::vec3 rt = GetExtentMax();
	glm::vec rayDirInv = glm::vec3(1.0f, 1.0f, 1.0f) / ray.Direction;
	
	float t1 = (lb.x - ray.Origin.x) * rayDirInv.x;
	float t2 = (rt.x - ray.Origin.x) * rayDirInv.x;

	float t3 = (lb.y - ray.Origin.y) * rayDirInv.y;
	float t4 = (rt.y - ray.Origin.y) * rayDirInv.y;

	float t5 = (lb.z - ray.Origin.z) * rayDirInv.z;
	float t6 = (rt.z - ray.Origin.z) * rayDirInv.z;

	float tmin = glm::max(glm::max(glm::min(t1, t2), glm::min(t3, t4)), glm::min(t5, t6));
	float tmax = glm::min(glm::min(glm::max(t1, t2), glm::max(t3, t4)), glm::max(t5, t6));

	// if tmax < 0, ray (line) is intersecting AABB, but the whole AABB is behind us
	if (tmax < 0)
	{
		return Trace().Miss();
	}

	// if tmin > tmax, ray doesn't intersect AABB
	if (tmin > tmax)
	{
		return Trace().Miss();
	}

	return Trace().Hit(tmin, Guid);
}

glm::vec3 Box::ComputeAABBNormal(const Ray& ray, glm::vec3 hitPosition) const
{
	glm::vec3 normal = glm::vec3(0.0f, 0.0f, 0.0f);// Initialize the normal vector

	// Check which face the intersection point lies on
	float epsilon = 1e-6;
	for (uint8_t i = 0; i < 3; i++)
	{
		if (abs(hitPosition[i] - GetExtentMin()[i]) < epsilon)
		{
			normal[i] = -1;//The intersection point lies on the minimum face in the i - th dimension
		}
		else if (abs(hitPosition[i] - GetExtentMax()[i]) < epsilon)
		{
			normal[i] = 1;//The intersection point lies on the maximum face in the i - th dimension
		}
	}

	return normal;
}

Trace Box::ResolveTracePayload(Trace& payload, const Ray& ray) const
{
	payload.WorldPosition = ray.Origin + ray.Direction * payload.HitDistance;
	payload.WorldNormal = ComputeAABBNormal(ray, payload.WorldPosition);
	return payload;
}
