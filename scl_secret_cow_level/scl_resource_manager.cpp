#include "scl_resource_manager.h"
#include "scl_pico_png.h"

#include <GL/glew.h>
#include <fstream>

std::vector<SCLTexture> SCLResourceManager::m_LoadedTextures;

bool SCLResourceManager::Initialize()
{
	m_LoadedTextures.resize(SCLResources::TextureType_SIZE);

	// Load all textures
	for (unsigned int TexI = 0; TexI < SCLResources::TextureType_SIZE; ++TexI)
	{
		SCLResources::TextureType TexType = (SCLResources::TextureType)TexI;
		m_LoadedTextures[TexI] = LoadPNGToTexture(SCLResources::GetTexturePath(TexType));
		if (m_LoadedTextures[TexI].GLTexId == 0)
		{
			return false;
		}
		m_LoadedTextures[TexI].SCLTexType = TexI;
	}

	return true;
}

SCLTexture SCLResourceManager::GetLoadedTexture(SCLResources::TextureType TexType)
{
	if (TexType < 0 || TexType >= SCLResources::TextureType_SIZE)
	{
		return SCLTexture();
	}
	return m_LoadedTextures[TexType];
}

SCLTexture SCLResourceManager::LoadPNGToTexture(const std::string& FilePath)
{
	SCLTexture NewTexture = SCLTexture();

	std::vector<unsigned char> In;
	if (!ReadFileToBuffer(FilePath, In))
	{
		printf("ERROR: Failed to load PNG file to buffer: %s\n", FilePath.c_str());
		return NewTexture;
	}

	// picoPNG! Just followed a tutorial..
	unsigned long Width, Height;
	std::vector<unsigned char> Out;
	int ErrorCode = decodePNG(Out, Width, Height, &(In[0]), In.size(), true);
	if (ErrorCode != 0)
	{
		printf("ERROR: decodePNG failed with error: %d\n", ErrorCode);
		return NewTexture;
	}

	glGenTextures(1, &(NewTexture.GLTexId));
	glBindTexture(GL_TEXTURE_2D, NewTexture.GLTexId);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, Width, Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, &(Out[0]));
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// NOTE: Nearest-neighbor is better for the pixel-art style I'm using
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
	glGenerateMipmap(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, 0);

	NewTexture.Width = Width;
	NewTexture.Height = Height;

	return NewTexture;
}

bool SCLResourceManager::ReadFileToBuffer(const std::string& FilePath, std::vector<unsigned char>& Buffer)
{
	// Open a file stream
	std::ifstream FileStream(FilePath, std::ios::binary);
	if (FileStream.fail())
	{
		printf("ERROR: Failed to open file: %s\n", FilePath.c_str());
		return false;
	}

	// Get the file size
	FileStream.seekg(0, std::ios::end);
	int FileSize = (int)(FileStream.tellg());
	FileStream.seekg(0, std::ios::beg);

	// Remove file header size
	FileSize -= (int)(FileStream.tellg());

	// Read the file
	Buffer.resize(FileSize);
	FileStream.read((char*)&(Buffer[0]), FileSize);
	FileStream.close();

	return true;
}