#pragma once

struct SCLTexture
{
	unsigned int SCLTexType;	// My own index for this texture
	unsigned int GLTexId;		// OpenGL index after loading
	unsigned int Width;
	unsigned int Height;
};