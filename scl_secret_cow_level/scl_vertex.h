#pragma once

#include <glm/glm.hpp>

struct SCLVertex
{
	glm::vec2 Position;
	glm::vec4 Color;
	glm::vec2 UV;

	void SetPosition(float x, float y)
	{
		Position = glm::vec2(x, y);
	}

	void SetColor(float r, float g, float b, float a)
	{
		Color = glm::vec4(r, g, b, a);
	}

	void SetUV(float u, float v)
	{
		UV = glm::vec2(u, v);
	}
};