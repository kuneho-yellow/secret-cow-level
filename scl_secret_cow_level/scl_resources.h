#pragma once

#include <string>

class SCLResources
{
public:
	// TODO: In current implementation this order
	// becomes the order of drawing..
	enum TextureType
	{
		TextureType_BG,
		TextureType_Grass1,
		TextureType_Grass2,
		TextureType_Grass3,
		TextureType_Bush1,
		TextureType_Rock1,
		TextureType_Pickup1,
		TextureType_Pickup2,
		TextureType_Pickup3,
		TextureType_Pickup4,
		TextureType_Cow1,
		TextureType_Cow2,
		TextureType_Cow3,
		TextureType_Player,
		TextureType_PlayerDead,
		TextureType_Bullet1,
		TextureType_Text,
		TextureType_SIZE
	};

	static std::string GetTexturePath(TextureType TexType);

private:
	static const std::string m_TexturePaths[];
};