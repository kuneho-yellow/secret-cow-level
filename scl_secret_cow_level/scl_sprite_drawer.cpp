#include "scl_sprite_drawer.h"
#include "scl_shader_program.h"
#include "scl_sprite.h"
#include "scl_resources.h"

SCLSpriteDrawer::SCLSpriteDrawer()
	:m_VAOId(0),
	m_VBOId(0)
{
	m_RenderBatches.resize(SCLResources::TextureType_SIZE);
}


SCLSpriteDrawer::~SCLSpriteDrawer()
{
}

bool SCLSpriteDrawer::Initialize()
{
	CreateVertexArrayObject();
	
	return true;
}

void SCLSpriteDrawer::DrawSprites(const std::vector<const SCLSprite*>* pSpritesToDraw)
{
	if (pSpritesToDraw->empty())
	{
		return;
	}

	// Group the sprites into "batches" according to shared texture
	CreateRenderBatches(pSpritesToDraw);

	// Draw the giant VAO (> u <)
	glBindVertexArray(m_VAOId);
	unsigned int Offset = 0;
	for (unsigned int BatchI = 0; BatchI < m_RenderBatches.size(); ++BatchI)
	{
		if (m_RenderBatches[BatchI].VertexCount > 0)
		{
			glBindTexture(GL_TEXTURE_2D, m_RenderBatches[BatchI].TextureId);
			glDrawArrays(GL_TRIANGLES, Offset, m_RenderBatches[BatchI].VertexCount);
			Offset += m_RenderBatches[BatchI].VertexCount;
		}
	}
	glBindTexture(GL_TEXTURE_2D, 0);
	glBindVertexArray(0);
}

void SCLSpriteDrawer::CreateVertexArrayObject()
{
	if (m_VAOId == 0)
	{
		glGenVertexArrays(1, &m_VAOId);
	}
	glBindVertexArray(m_VAOId);

	if (m_VBOId == 0)
	{
		glGenBuffers(1, &m_VBOId);
	}
	glBindBuffer(GL_ARRAY_BUFFER, m_VBOId);

	// Set Attributes
	// Poisition attribute
	glEnableVertexAttribArray(SCLShaderProgram::ATTRB_VERTEX_POSITION);
	glVertexAttribPointer(SCLShaderProgram::ATTRB_VERTEX_POSITION, 2, GL_FLOAT, GL_FALSE, sizeof(SCLVertex), (void*)offsetof(SCLVertex, Position));
	// Color attribute
	glEnableVertexAttribArray(SCLShaderProgram::ATTRB_VERTEX_COLOR);
	glVertexAttribPointer(SCLShaderProgram::ATTRB_VERTEX_COLOR, 4, GL_FLOAT, GL_FALSE, sizeof(SCLVertex), (void*)offsetof(SCLVertex, Color));
	// UV attribute
	glEnableVertexAttribArray(SCLShaderProgram::ATTRB_VERTEX_UV);
	glVertexAttribPointer(SCLShaderProgram::ATTRB_VERTEX_UV, 2, GL_FLOAT, GL_FALSE, sizeof(SCLVertex), (void*)offsetof(SCLVertex, UV));
	
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void SCLSpriteDrawer::CreateRenderBatches(const std::vector<const SCLSprite*>* pSpritesToDraw)
{
	if (pSpritesToDraw->empty())
	{
		return;
	}

	// TODO: USE INDEX BUFFER :I
	const int VertexCount = 6; // 4 corners of the sprite

	// Create a giant array of vertices to upload
	std::vector<SCLVertex> AllVertices;
	AllVertices.resize(pSpritesToDraw->size() * VertexCount);

	// Poopulate m_RenderBatches
	m_RenderBatches.clear();
	for (unsigned int TxtI = 0; TxtI < SCLResources::TextureType_SIZE; ++TxtI)
	{
		m_RenderBatches.push_back(RenderBatch(0, 0));
	}
	for (unsigned int SprI = 0; SprI < pSpritesToDraw->size(); ++SprI)
	{
		int BatchI = (*pSpritesToDraw)[SprI]->Texture.SCLTexType;
		m_RenderBatches[BatchI].TextureId = (*pSpritesToDraw)[SprI]->Texture.GLTexId;
		m_RenderBatches[BatchI].VertexCount += VertexCount;
		m_RenderBatches[BatchI].Sprites.push_back((*pSpritesToDraw)[SprI]);
	}
	// Populate the giant vertex array
	unsigned int VertI = 0;
	for (unsigned int BatchI = 0; BatchI < m_RenderBatches.size(); ++BatchI)
	{
		for (unsigned int SprI = 0; SprI < m_RenderBatches[BatchI].Sprites.size(); ++SprI)
		{
			AllVertices[VertI++] = m_RenderBatches[BatchI].Sprites[SprI]->VertTopLeft;
			AllVertices[VertI++] = m_RenderBatches[BatchI].Sprites[SprI]->VertTopRight;
			AllVertices[VertI++] = m_RenderBatches[BatchI].Sprites[SprI]->VertBotLeft;
			AllVertices[VertI++] = m_RenderBatches[BatchI].Sprites[SprI]->VertBotLeft;
			AllVertices[VertI++] = m_RenderBatches[BatchI].Sprites[SprI]->VertTopRight;
			AllVertices[VertI++] = m_RenderBatches[BatchI].Sprites[SprI]->VertBotRight;
		}
	}

	glBindBuffer(GL_ARRAY_BUFFER, m_VBOId);
	// Orphan the buffer --- TODO: must understand what this is (T o T)
	glBufferData(GL_ARRAY_BUFFER, AllVertices.size() * sizeof(SCLVertex), NULL, GL_DYNAMIC_DRAW);
	// Upload the data
	glBufferSubData(GL_ARRAY_BUFFER, 0, AllVertices.size() * sizeof(SCLVertex), &(AllVertices[0]));
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}