#pragma once

#include <GL/glew.h>
#include <vector>

// #include "scl_sprite.h"

struct SCLSprite;

class SCLSpriteDrawer
{
public:
	SCLSpriteDrawer();
	~SCLSpriteDrawer();

	bool Initialize();
	void DrawSprites(const std::vector<const SCLSprite*>* pSpritesToDraw);

private:

	struct RenderBatch
	{
		RenderBatch()
			: VertexCount(0),
			TextureId(0),
			Sprites()
		{
		}
		RenderBatch(GLuint VertexCount, GLuint TextureId)
			: VertexCount(VertexCount),
			TextureId(TextureId),
			Sprites()
		{
		}
		GLuint VertexCount;
		GLuint TextureId;
		std::vector<const SCLSprite*> Sprites;
	};

	void CreateVertexArrayObject();
	void CreateRenderBatches(const std::vector<const SCLSprite*>* pSpritesToDraw);

	std::vector<RenderBatch> m_RenderBatches;
	GLuint m_VAOId;
	GLuint m_VBOId;
};