#pragma once

#include "scl_game_object.h"

#include <glm/glm.hpp>

class SCLGrid;
struct SCLTexture;

class SCLEnemy : public SCLGameObject
{
public:
	SCLEnemy();
	~SCLEnemy();

	void Initialize(unsigned int Type, unsigned int EnemyIndex, glm::vec2 InitPos);
	void Update(float DeltaTime);
	void FixedUpdate(float FixedDeltaTime);
	void NotifyCollision(SCLGameObject& OtherGO);

	void SetEnemyGrid(SCLGrid* pEnemyGrid);
	void SetMapGrid(SCLGrid* pMapGrid);
	void SetPlayer(SCLGameObject* pPlayer);

	bool IsDead() const;
	unsigned int GetScore() const; // Score added when defeated
	bool IsSpawnPickup() const; // Depends on enemy type

private:
	bool CheckHitMoveBounds();
	void CheckCollisionsWithMap(float FixedDeltaTime);
	void CheckCollisionsWithEnemies(float FixedDeltaTime);
	void ProcessHitByBullet();

	// TODO: Do these properly somewhere else
	static unsigned int GetMaxHPFromType(unsigned int Type);
	static unsigned int GetScoreFromType(unsigned int Type);
	static float GetMoveSpeedFromType(unsigned int Type);
	static SCLTexture GetTextureFromType(unsigned int Type);
	static bool GetPickupChanceFromType(unsigned int Type);

	unsigned int m_Type;
	unsigned int m_EnemyIndex;
	unsigned int m_HP;
	unsigned int m_Score;
	glm::vec2 m_MoveDir;
	float m_MoveSpeed;
	SCLGrid* m_pEnemyGrid;
	SCLGrid* m_pMapGrid;
	SCLGameObject* m_pPlayer;
};

