#pragma once

#include "scl_game_object.h"
#include "scl_texture.h"

#include <glm/glm.hpp>

class SCLPlayer;
class SCLGrid;

class SCLBullet : public SCLGameObject
{
public:
	SCLBullet();
	~SCLBullet();

	void Initialize(SCLTexture Texture, float Size, unsigned int BulletIndex,
					glm::vec2 InitPos, glm::vec2 MoveDir,
					float MoveSpeed, float DistMax);

	void Update(float DeltaTime);
	void FixedUpdate(float FixedDeltaTime);
	void NotifyCollision(SCLGameObject& OtherGO);

	void SetEnemyGrid(SCLGrid* pEnemyGrid);
	void SetMapGrid(SCLGrid* pMapGrid);

private:
	void CheckCollisionsWithMap(float FixedDeltatTime);
	void CheckCollisionsWithEnemies(float FixedDeltatTime);

	unsigned int m_BulletIndex;
	glm::vec2 m_MoveDir;
	float m_MoveSpeed;
	float m_MoveDistMax;
	float m_MoveTimeMax;
	float m_MoveTimer;
	SCLGrid* m_pEnemyGrid;
	SCLGrid* m_pMapGrid;
};

