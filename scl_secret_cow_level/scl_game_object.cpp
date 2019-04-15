#include "scl_game_object.h"
#include "scl_grid.h"

SCLGameObject::SCLGameObject()
	: m_IsActive(false),
	m_Transform(),
	m_Sprite(),
	m_pCell(NULL),
	m_CellVectorI(-1),
	m_ColliderRadius(0.0f),
	m_CollisionLayer(LAYER_DEFAULT)
{

}

SCLGameObject::~SCLGameObject()
{

}

void SCLGameObject::Update(float DeltaTime)
{

}

void SCLGameObject::FixedUpdate(float FixedDeltaTime)
{

}

bool SCLGameObject::IsActive() const
{
	return m_IsActive;
}

const glm::vec2 SCLGameObject::GetPosition() const
{
	return m_Transform.GetPosition();
}

void SCLGameObject::SetPosition(glm::vec2 NewPos)
{
	glm::vec2 Translation = NewPos - m_Transform.GetPosition();
	m_Transform.SetPosition(NewPos);
	UpdateSpriteVertices(Translation);
}

void SCLGameObject::Move(glm::vec2 DeltaPos)
{
	m_Transform.Translate(DeltaPos);
	UpdateSpriteVertices(DeltaPos);
}

void SCLGameObject::SetSprite(SCLTexture Texture, float Size)
{
	m_Sprite.Texture = Texture;

	float SizeRatio = (float)m_Sprite.Texture.Height / m_Sprite.Texture.Width;
	float WidthHalf = Size * 0.5f;
	float HeightHalf = WidthHalf * SizeRatio;

	m_Sprite.VertTopLeft.SetColor(1.0f, 1.0f, 1.0f, 1.0f);
	m_Sprite.VertTopLeft.SetPosition(-WidthHalf, HeightHalf);
	m_Sprite.VertTopLeft.SetUV(0.0f, 1.0f);

	m_Sprite.VertBotLeft.SetColor(1.0f, 1.0f, 1.0f, 1.0f);
	m_Sprite.VertBotLeft.SetPosition(-WidthHalf, -HeightHalf);
	m_Sprite.VertBotLeft.SetUV(0.0f, 0.0f);

	m_Sprite.VertTopRight.SetColor(1.0f, 1.0f, 1.0f, 1.0f);
	m_Sprite.VertTopRight.SetPosition(WidthHalf, HeightHalf);
	m_Sprite.VertTopRight.SetUV(1.0f, 1.0f);

	m_Sprite.VertBotRight.SetColor(1.0f, 1.0f, 1.0f, 1.0f);
	m_Sprite.VertBotRight.SetPosition(WidthHalf, -HeightHalf);
	m_Sprite.VertBotRight.SetUV(1.0f, 0.0f);

	UpdateSpriteVertices(m_Transform.GetPosition());
}

const SCLSprite* SCLGameObject::GetSprite() const
{
	return &m_Sprite;
}

void SCLGameObject::SetSpriteAlpha(float Alpha)
{
	m_Sprite.VertTopLeft.SetColor(1.0f, 1.0f, 1.0f, Alpha);
	m_Sprite.VertBotLeft.SetColor(1.0f, 1.0f, 1.0f, Alpha);
	m_Sprite.VertTopRight.SetColor(1.0f, 1.0f, 1.0f, Alpha);
	m_Sprite.VertBotRight.SetColor(1.0f, 1.0f, 1.0f, Alpha);
}

void SCLGameObject::SetSpriteDirection(bool IsRight)
{
	// For flipping the sprite
	if (IsRight)
	{
		m_Sprite.VertTopLeft.SetUV(0.0f, 1.0f);
		m_Sprite.VertBotLeft.SetUV(0.0f, 0.0f);
		m_Sprite.VertTopRight.SetUV(1.0f, 1.0f);
		m_Sprite.VertBotRight.SetUV(1.0f, 0.0f);
	}
	else
	{
		m_Sprite.VertTopLeft.SetUV(1.0f, 1.0f);
		m_Sprite.VertBotLeft.SetUV(1.0f, 0.0f);
		m_Sprite.VertTopRight.SetUV(0.0f, 1.0f);
		m_Sprite.VertBotRight.SetUV(0.0f, 0.0f);
	}
}

void SCLGameObject::SetUV(float BotLeftU, float BotLeftV, float TopRightU, float TopRightV)
{
	m_Sprite.VertTopLeft.SetUV(BotLeftU, TopRightV);
	m_Sprite.VertBotLeft.SetUV(BotLeftU, BotLeftV);
	m_Sprite.VertTopRight.SetUV(TopRightU, TopRightV);
	m_Sprite.VertBotRight.SetUV(TopRightU, BotLeftV);
}

SCLCell* SCLGameObject::GetCellPointer() const
{
	return m_pCell;
}

void SCLGameObject::SetCellPointer(SCLCell* pCell)
{
	m_pCell = pCell;
}

int SCLGameObject::GetCellVectorI() const
{
	return m_CellVectorI;
}

void SCLGameObject::SetCellVectorI(int CellVectorI)
{
	m_CellVectorI = CellVectorI;
}

float SCLGameObject::GetColliderRadius() const
{
	return m_ColliderRadius;
}

void SCLGameObject::SetColliderRadius(float Radius)
{
	m_ColliderRadius = abs(Radius);
}

void SCLGameObject::SetCollisionLayer(SCLCollisionLayer Layer)
{
	m_CollisionLayer = Layer;
}

SCLCollisionLayer SCLGameObject::GetCollisionLayer() const
{
	return m_CollisionLayer;
}

bool SCLGameObject::CheckCollision(SCLGameObject& OtherGO)
{
	// Such simple collision check...
	glm::vec2 DistVec = m_Transform.GetPosition() - OtherGO.GetPosition();
	float MinColDist = m_ColliderRadius + OtherGO.GetColliderRadius();
	float CurDistSqr = (DistVec.x * DistVec.x) + (DistVec.y * DistVec.y);

	if (CurDistSqr < (MinColDist * MinColDist))
	{
		NotifyCollision(OtherGO);
		// Want to control if partner gets notified too
		// OtherGO.NotifyCollision(*this);
		return true;
	}
	return false;
}

void SCLGameObject::NotifyCollision(SCLGameObject& OtherGO)
{

}

void SCLGameObject::AdjustCollisionDepthHalf(SCLGameObject& OtherGO)
{
	// Move away from colliding partner by half of what it takes to "uncollide"
	glm::vec2 DistVec = m_Transform.GetPosition() - OtherGO.GetPosition();
	float Dist = glm::length(DistVec);
	float MinDist = m_ColliderRadius + OtherGO.GetColliderRadius();
	float CollisionDepth = MinDist - Dist;
	if (CollisionDepth > 0)
	{
		glm::vec2 DirVec = glm::normalize(DistVec);
		Move(DirVec * CollisionDepth * 0.5f);
		// Then make partner do the other half
		//OtherGO.Move(-DirVec * CollisionDepth * 0.5f);
	}
}

void SCLGameObject::AdjustCollisionDepthFull(SCLGameObject& OtherGO)
{
	glm::vec2 DistVec = m_Transform.GetPosition() - OtherGO.GetPosition();
	float Dist = glm::length(DistVec);
	float MinDist = m_ColliderRadius + OtherGO.GetColliderRadius();
	float CollisionDepth = MinDist - Dist;
	if (CollisionDepth > 0)
	{
		glm::vec2 DirVec = glm::normalize(DistVec);
		Move(DirVec * CollisionDepth);
	}
}

void SCLGameObject::UpdateSpriteVertices(glm::vec2 DeltaPos)
{
	// Directly editing the vertices because world matrix isn't passed to the shader
	m_Sprite.VertTopLeft.Position += DeltaPos;
	m_Sprite.VertBotLeft.Position += DeltaPos;
	m_Sprite.VertBotRight.Position += DeltaPos;
	m_Sprite.VertTopRight.Position += DeltaPos;
}