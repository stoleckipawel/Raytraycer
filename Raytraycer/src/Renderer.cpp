#include "Renderer.h"
#include "Walnut/Random.h"
#include <execution>

static uint32_t ConvertColorToUInt32(glm::vec4 color)
{
	color = glm::clamp(color, glm::vec4(0.0f), glm::vec4(1.0f));
	color *= 255.0f;
	return ((uint8_t)color.x) | ((uint8_t)color.y << 8) | ((uint8_t)color.z << 16) | ((uint8_t)color.w << 24);
}

void Renderer::Render(const Scene& scene, const Camera& camera)
{
	m_ActiveCamera = &camera;
	m_ActiveScene = &scene;

	if (m_ActiveScene->Spheres.size() == 0.0)
		return;
	
	if (m_FrameIndex == 1)
		memset(m_HistoryBufferData, 0, m_FrontBuffer->GetWidth() * m_FrontBuffer->GetHeight() * sizeof(glm::vec4));//clear buffer

	std::for_each(std::execution::par, m_VerticalPixelsIterator.begin(), m_VerticalPixelsIterator.end(), 
		[this](uint32_t y) 
		{
			std::for_each(std::execution::par, m_HorizontalPixelsIterator.begin(), m_HorizontalPixelsIterator.end(),
			[this, y](uint32_t x)
			{
				uint32_t pixel_id = x + y * m_FrontBuffer->GetWidth();
				m_HistoryBufferData[pixel_id] += RayGen(x, y);
				glm::vec4 accumulated_color = m_HistoryBufferData[pixel_id] / (float)m_FrameIndex;
				m_FrontBufferData[pixel_id] = ConvertColorToUInt32(accumulated_color);
			});
		});

	m_FrontBuffer->SetData(m_FrontBufferData);

	if (m_Settings.Accumulate)
	{
		m_FrameIndex++;
	}
	else
	{
		m_FrameIndex = 1;
	}
}

void Renderer::OnResize(uint32_t width, uint32_t height)
{
	if (!m_FrontBuffer)
	{
		m_FrontBuffer = std::make_shared<Walnut::Image>(width, height, Walnut::ImageFormat::RGBA);
	}
	else if (width != m_FrontBuffer->GetWidth() || height != m_FrontBuffer->GetHeight())
	{
		m_FrontBuffer->Resize(width, height);
	}
	else
	{
		return;
	}

	delete[] m_FrontBufferData;
	m_FrontBufferData = new uint32_t[width * height];

	delete[] m_HistoryBufferData;
	m_HistoryBufferData = new glm::vec4[width * height];

	m_FrameIndex = 1;

	m_VerticalPixelsIterator.resize(height);
	for (uint32_t i = 0; i < height; i++)
	{
		m_VerticalPixelsIterator[i] = i;
	}
	m_HorizontalPixelsIterator.resize(width);
	for (uint32_t i = 0; i < width; i++)
	{
		m_HorizontalPixelsIterator[i] = i;
	}
}

Renderer::HitPayload Renderer::TraceRay(const Ray& ray)
{
	//Ray sphere intersection
	//bx^2 +by^2)t^2 + (2axbx + 2ayby)t + (ax^2 + ay^2 - r^2) = 0
	//a = ray origin
	//b = ray direction
	//r = sphere radius

	size_t closestSphere_id = -1;
	float closestHitDistance = 9999.0;

	for (size_t i = 0; i < m_ActiveScene->Spheres.size(); i++)
	{
		const Sphere& sphere = m_ActiveScene->Spheres[i];
		float radiusSquared = sphere.Radius * sphere.Radius;
		glm::vec3 origin = ray.Origin - sphere.Position;//optimization for sphere placement

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
			if (hitDistance > 0.0f && hitDistance < closestHitDistance)
			{
				closestHitDistance = hitDistance;
				closestSphere_id = (int)i;
			}
		}
	}

	if (closestSphere_id == -1)
	{
		return Miss(ray);
	}
	else
	{
		return ClosestHit(ray, closestSphere_id, closestHitDistance);
	}
}

glm::vec4 Renderer::RayGen(uint32_t x, uint32_t y)
{
	
	Ray ray;
	ray.Origin = m_ActiveCamera->GetPosition();
	ray.Direction = m_ActiveCamera->GetRayDirections()[x + y * m_FrontBuffer->GetWidth()];

	glm::vec3 lightDir = glm::vec3(-1.0f, -1.0f, -1.0f);
	lightDir = glm::normalize(lightDir);

	glm::vec3 color = glm::vec3(0.0f, 0.0f, 0.0f);
	float bounce_multiplier = 1.0f;
	int bounces = 4;
	for (int i = 0; i < bounces; i++)
	{
		Renderer::HitPayload payload = TraceRay(ray);

		if (payload.HitDistance == -1.0f)
		{
			color += glm::vec3(0.3, 0.7, 1.0) * bounce_multiplier;
			break;
		}
		else
		{
			const Sphere& sphere = m_ActiveScene->Spheres[payload.ObjectIndex];
			const Material& material = m_ActiveScene->Materials[sphere.MaterialIndex];
			
			for (int i = 0; i < m_ActiveScene->DirectionalLights.size(); i++)
			{
				const DirectionalLight& directionalLight = m_ActiveScene->DirectionalLights[i];
				glm::vec3 lightDir = glm::normalize(directionalLight.Direction);

				Ray directSunRay;
				directSunRay.Origin = payload.WorldPosition - (lightDir * FLT_MIN);
				directSunRay.Direction = -lightDir;
				Renderer::HitPayload sunRayPayload = TraceRay(directSunRay);
				if (sunRayPayload.HitDistance == -1.0)//if there are no blockers
				{
					float NoL = glm::max(glm::dot(payload.WorldNormal, -lightDir), 0.0f);
					glm::vec3 lightColor = directionalLight.Color;
					float lightIntensity = directionalLight.Intensity;
					color += material.Albedo * lightColor * lightIntensity * NoL * bounce_multiplier;
				}
			}
		}

		ray.Origin = payload.WorldPosition + payload.WorldNormal * FLT_MIN;//biased

		const Sphere& sphere = m_ActiveScene->Spheres[payload.ObjectIndex];
		const Material& material = m_ActiveScene->Materials[sphere.MaterialIndex];
		ray.Direction = glm::reflect(ray.Direction, payload.WorldNormal + 
			material.Roughness * Walnut::Random::Vec3(-0.5f,0.5f));

		bounce_multiplier *= 0.333f;
	}

	return glm::vec4(color, 1.0f);

}

Renderer::HitPayload Renderer::ClosestHit(const Ray& ray, uint32_t objectIndex, float hitDistance)
{
	Renderer::HitPayload payload;
	payload.HitDistance = hitDistance;
	payload.ObjectIndex = objectIndex;

	const Sphere& closestSphere = m_ActiveScene->Spheres[objectIndex];
	glm::vec3 origin = ray.Origin - closestSphere.Position;//optimization for sphere placement
	payload.WorldPosition = origin + ray.Direction * hitDistance;
	payload.WorldNormal = glm::normalize(payload.WorldPosition);//assumes that each sphere is in the center of the world
	payload.WorldPosition += closestSphere.Position;
	return payload;
}

Renderer::HitPayload Renderer::Miss(const Ray& ray)
{
	Renderer::HitPayload payload;
	payload.HitDistance = -1.0f;
	return payload;
}

