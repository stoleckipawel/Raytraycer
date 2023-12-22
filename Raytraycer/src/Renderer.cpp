#include "Renderer.h"
#include "Walnut/Random.h"

void Renderer::Render()
{
	for (uint32_t y = 0; y < m_FrontBuffer->GetHeight(); y++)
	{
		for (uint32_t x = 0; x < m_FrontBuffer->GetWidth(); x++)
		{
			glm::vec2 coord = glm::vec2((float)x / (float)m_FrontBuffer->GetWidth(), (float)y / (float)m_FrontBuffer->GetHeight());
			m_FrontBufferData[x + y * m_FrontBuffer->GetWidth()] = PerPixel(coord);
		}
	}

	m_FrontBuffer->SetData(m_FrontBufferData);
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

}

uint32_t Renderer::PerPixel(glm::vec2 coord)
{
	uint32_t r = coord.x * 255.0f;
	uint32_t g = coord.y * 255.0f;
	return 0xff000000 | (g << 8) | r;
}

