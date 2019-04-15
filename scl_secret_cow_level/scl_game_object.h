#pragma once

#include "scl_tansform.h"
#include "scl_sprite.h"

struct SCLTexture;
struct SCLCell;

enum SCLCollisionLayer
{
	LAYER_DEFAULT,
	LAYER_PLAYER,
	LAYER_ENEMY,
	LAYER_MAP,
	LAYER_BULLET,
	LAYER_PICKUP
};

class SCLGameObject
{
public:
	SCLGameObject();
	virtual ~SCLGameObject();

	virtual void Update(float DeltaTime);
	virtual void FixedUpdate(float FixedDeltaTime);
	bool IsActive() const;

	// Movement
	const glm::vec2 GetPosition() const;
	void SetPosition(glm::vec2 NewPos);
	void Move(glm::vec2 DeltaPos);

	// Sprite
	void SetSprite(SCLTexture Texture, float Size);
	const SCLSprite* GetSprite() const;
	void SetSpriteAlpha(float Alpha);
	void SetSpriteDirection(bool IsRight);
	void SetUV(float BotLeftU, float BotLeftV, float TopRightU, float TopRightV);

	// Grid
	SCLCell* GetCellPointer() const;
	void SetCellPointer(SCLCell* pCell);
	int GetCellVectorI() const;
	void SetCellVectorI(int CellVectorI);
	
	// Collisions
	float GetColliderRadius() const;
	void SetColliderRadius(float Radius);
	void SetCollisionLayer(SCLCollisionLayer Layer);
	SCLCollisionLayer GetCollisionLayer() const;
	bool SCLGameObject::CheckCollision(SCLGameObject& OtherGO);
	virtual void NotifyCollision(SCLGameObject& OtherGO);
	void AdjustCollisionDepthHalf(SCLGameObject& OtherGO);
	void AdjustCollisionDepthFull(SCLGameObject& OtherGO);

protected:
	void UpdateSpriteVertices(glm::vec2 DeltaPos);

	bool m_IsActive;
	SCLTransform m_Transform;
	SCLSprite m_Sprite;

	// Grid
	SCLCell* m_pCell;
	int m_CellVectorI;

	// Circle Collider
	float m_ColliderRadius;
	SCLCollisionLayer m_CollisionLayer;
};