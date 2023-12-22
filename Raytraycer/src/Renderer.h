#pragma once

#include "Walnut/Image.h"
#include <memory>

class Renderer
{
public:
	Renderer() = default;
	void Render();
	void OnResize(uint32_t width, uint32_t height);
	std::shared_ptr<Walnut::Image> GetFrontBuffer() const { return m_FrontBuffer; }
private:
	std::shared_ptr<Walnut::Image> m_FrontBuffer;
	uint32_t* m_FrontBufferData = nullptr;
};

