#pragma once

#include "scl_resources.h"
#include "scl_texture.h"

#include <vector>

class SCLResourceManager
{
public:
	static bool Initialize();
	static SCLTexture GetLoadedTexture(SCLResources::TextureType TexType);

private:
	static SCLTexture LoadPNGToTexture(const std::string& FilePath);
	static bool ReadFileToBuffer(const std::string& FilePath, std::vector<unsigned char>& Buffer);

	static std::vector<SCLTexture> m_LoadedTextures;
};

