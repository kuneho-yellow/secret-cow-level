#pragma once

#include "scl_game_object.h"
#include "scl_move_dir.h"

#include <glm/glm.hpp>

class SCLGameManager;
class SCLBullet;
struct SCLTexture;

class SCLPlayer : public SCLGameObject
{
public:
	SCLPlayer();
	~SCLPlayer();

	void Initialize(SCLGameManager* pGameManager);

	// SCLGameObject Overrides
	void Update(float DeltaTime);
	void FixedUpdate(float FixedDeltaTime);
	void NotifyCollision(SCLGameObject& OtherGO);

	// Info
	unsigned int GetHP() const;

	// Input
	void AddMoveDir(SCLMoveDir Dir);
	void RemoveMoveDir(SCLMoveDir Dir);
	void UpdateFiringState(bool IsFiring);
	void UpdateFiringPosition(glm::vec2 FiringPos);
	void RotateFiringDir(bool IsClockwise);
	void StopRotateFiringDir();

private:
	bool UpdateMoveDir(); // Returns true if there is movement expected
	void CreateBullet();
	void UpdateBullets(float DeltaTime);
	void UpdateBulletCollisions(float FixedDeltaTime);
	glm::vec2 GetRandomFiringDir();
	void UpdateFiringDirRotation(float DeltaTime);

	void CheckCollisionsWithEnemies(float FixedDeltaTime);
	void ProcessHitByEnemy();
	void ProcessGetPickup(SCLGameObject& Pickup);
	void ProcessDeath();
	void StartInvincibility();
	void EndInvincibility();
	void StartSuperMode();
	void EndSuperMode();

	void CheckCollisionsWithMap(float FixedDeltaTime);

	SCLGameManager* m_pGameManager;
	SCLBullet* m_pBullets;
	unsigned int m_BulletCurrentIndex;
	// Movement
	glm::vec2 m_MoveDir;
	bool m_MoveStates[MOVE_DIR_SIZE];
	// Attack
	glm::vec2 m_FiringPos;
	glm::vec2 m_FiringDir;
	float m_FiringTimer;
	float m_FiringConeAngle;
	float m_FiringSpeed;
	bool m_IsFiring;
	int m_RotateState;
	// Special
	bool m_IsSuperMode;
	float m_SuperModeTimer;
	float m_SuperModeFiringSpeed;
	float m_SuperModeFiringConeAngle;
	// Life
	unsigned int m_HP;
	bool m_IsInvincible;
	float m_InvincibleTimer;
};