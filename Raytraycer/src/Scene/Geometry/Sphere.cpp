#include "Sphere.h"

void Sphere::BuildUI(uint32_t id)
{
	ImGui::PushID(id);
	ImGui::DragFloat3("Position", glm::value_ptr(Position), 0.01f);
	ImGui::DragFloat("Radius", &Radius, 0.01f);
	ImGui::Separator();
	ImGui::PopID();
}

Trace Sphere::GeometricSolution(const Ray& ray) const
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

Trace Sphere::AnalyticSolution(const Ray& ray) const
{	
	//O - Ray Origin
	//D - Ray Direciton
	//t - ray distance
	//R - sphere Radius
	//C - sphere Position
	
	//Sphere ray intersection
	// |O + tD|^2 - R^2 = 0
	// O^2 + (tD)^2 + 2ODt - R^2 = 0
	// O^2 + D^2t^2 + 2ODt - R^2 = 0
	// D^2T^2 + 2ODt - (O^2 - R^2) = 0 <-- quadriatic equation
	// quadriatic equation = ax^2 + bx + c
	// a = D^2
	// b = 2OD
	// c = O^2 - R ^2
	// delta = b^2 - 4ac
	// if(delta > 0.0)
	//  tClosest = (-b - sqrt) / 2a
	//  tFurther = (-b + sqrt) / 2a
	// else if(delta == 0.0)
	//	tClosest = -b/2a
	// else no hit 

	//Once Sphere Center is added 
	// |O + tD^2 - C|^2 - R^2 = 0
	// a = D^2 = 1 (since we assume normalized RayDirection)
	// b = 2D(O-C)
	// c = |O-C|^2 - R^2

	//This assumes Ray Dir is normalized -> A = 1.0
	glm::vec3 L = ray.Origin - Position;
	float b = 2.0f * glm::dot(ray.Direction, L);
	float c = glm::dot(L, L) - (Radius * Radius);
	float discriminant = (b * b) - 4 * c;//(delta)
	if (discriminant < 0.0f)
	{
		return Trace().Miss();
	}
	else if (discriminant > 0.0f)
	{
		float closerT = (-b - glm::sqrt(discriminant)) / 2.0f;
		//float furtherT = (-b + glm::sqrt(discriminant)) / 2.0f;
		return Trace().Hit(closerT, Guid);
	}
	else
	{
		float T = -b / 2.0f;
		return Trace().Hit(T, Guid);
	}
}

Trace Sphere::TraceRay(const Ray& ray) const
{
	return AnalyticSolution(ray);
	//return GeometricSolution(ray);
}

Trace Sphere::ResolveTracePayload(Trace& trace, const Ray& ray) const
{
	trace.WorldPosition = ray.Origin + ray.Direction * trace.HitDistance;
	trace.WorldNormal = glm::normalize(trace.WorldPosition - Position);
	return trace;
}



