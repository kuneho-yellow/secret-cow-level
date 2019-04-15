#pragma once

#include "scl_input_manager.h"
#include "scl_player.h"
#include "scl_enemy_manager.h"
#include "scl_map_manager.h"
#include "scl_ui_manager.h"
#include "scl_move_dir.h"

#include <glm/glm.hpp>
#include <vector>

class SCLMain;
class SCLCamera;

class SCLGameManager
{
public:
	SCLGameManager();
	~SCLGameManager();

	bool Initialize(SCLMain* pMain);
	void StartGame(SCLCamera* pCamera);
	void Update(float DeltaTime);
	void FixedUpdate(float FixedDeltaTime);

	// Events
	void NotifyQuitPressed();
	void NotifyEndGame();
	void NotifyEnemyDeath(const SCLEnemy* pEnemy);

	// Player
	glm::vec2 GetPlayerPosition() const;
	void MovePlayer(SCLMoveDir Dir);
	void StopPlayer(SCLMoveDir Dir);
	void RotatePlayerFiringDir(bool IsClockwise);
	void StopRotatePlayerFiringDir();
	void SetPlayerAttackState(bool IsAttacking);
	void SetPlayerAttackPos(glm::vec2 AttackPos);

	// For collision checks
	SCLGameObject* GetPlayerGameObject();
	SCLGrid* GetEnemyGrid();
	SCLGrid* GetMapGrid();

	// Visibility
	const SCLCamera* GetCamera() const;
	void MoveCamera(glm::vec2 Displacement);
	glm::vec2 GetWorldBoundsX() const;
	glm::vec2 GetWorldBoundsY() const;
	const std::vector<const SCLSprite*>* GetSprites() const;
	void AddSprite(const SCLSprite* pSprite);

	// Game Info
	float GetFPS() const;
	unsigned int GetScore() const;
	unsigned int GetPlayerHP() const;

private:
	SCLMain* m_pMain;
	SCLInputManager m_InputManager;
	SCLCamera* m_pCamera;
	SCLPlayer m_Player;
	SCLEnemyManager m_EnemyManager;
	SCLMapManager m_MapManager;
	SCLUIManager m_UIManager;

	std::vector<const SCLSprite*> m_Sprites;

	unsigned int m_Score;
};

