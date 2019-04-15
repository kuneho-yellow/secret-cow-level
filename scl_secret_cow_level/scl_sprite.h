#pragma once

#include "scl_vertex.h"
#include "scl_texture.h"

struct SCLSprite
{
	SCLTexture Texture;

	SCLVertex VertTopLeft;
	SCLVertex VertBotLeft;
	SCLVertex VertTopRight;
	SCLVertex VertBotRight;
};