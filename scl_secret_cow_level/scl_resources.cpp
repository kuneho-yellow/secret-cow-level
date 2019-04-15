#include "scl_resources.h"

const std::string SCLResources::m_TexturePaths[] =
{
	// TODO: In current implementation this order
	// becomes the order of drawing..
	"files/textures/bg_1.png",
	"files/textures/grass_1.png",
	"files/textures/grass_2.png",
	"files/textures/grass_3.png",
	"files/textures/bush_1.png",
	"files/textures/rock_1.png",
	"files/textures/apple.png",
	"files/textures/banana.png",
	"files/textures/eggplant.png",
	"files/textures/pineapple.png",
	"files/textures/cow_1.png",
	"files/textures/cow_2.png",
	"files/textures/cow_3.png",
	"files/textures/chick.png",
	"files/textures/chick_dead.png",
	"files/textures/bullet_1.png",
	"files/textures/text.png"
};

std::string SCLResources::GetTexturePath(TextureType TexType)
{
	if (TexType < 0 || TexType >= TextureType_SIZE)
	{
		return "";
	}
	return SCLResources::m_TexturePaths[TexType];
}