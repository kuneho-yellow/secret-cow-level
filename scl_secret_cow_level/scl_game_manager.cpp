#include "scl_game_manager.h"
#include "scl_resource_manager.h"
#include "scl_game_constants.h"
#include "scl_enemy.h"
#include "scl_camera.h"
#include "scl_main.h"
#include "scl_random.h"

SCLGameManager::SCLGameManager()
	: m_pMain(NULL),
	m_InputManager(),
	m_pCamera(NULL),
	m_Player(),
	m_EnemyManager(),
	m_MapManager(),
	m_UIManager(),
	m_Sprites(),
	m_Score(0)
{
	
}


SCLGameManager::~SCLGameManager()
{
}

bool SCLGameManager::Initialize(SCLMain* pMain)
{
	m_pMain = pMain;
	m_InputManager.SetGameManager(this);
	return true;
}

void SCLGameManager::StartGame(SCLCamera* pCamera)
{
	m_pCamera = pCamera;

	// Initialize modules
	m_MapManager.Initialize(this);
	m_Player.Initialize(this);
	m_EnemyManager.Initialize(this);
	m_UIManager.Initialize(this);
}

// Update non-physics things
void SCLGameManager::Update(float DeltaTime)
{
	// Clear the drawing slate
	m_Sprites.clear();

	// Update the modules
	m_InputManager.UpdateInput();
	m_MapManager.Update(DeltaTime);
	m_Player.Update(DeltaTime);
	m_EnemyManager.Update(DeltaTime);
	m_UIManager.Update(DeltaTime);
}

// Update the physics
void SCLGameManager::FixedUpdate(float FixedDeltaTime)
{
	// TODO: Physics not yet totally separate from everything else D:
	// FixedDeltaTime not used yet
	m_MapManager.FixedUpdate(FixedDeltaTime);
	m_Player.FixedUpdate(FixedDeltaTime);
	m_EnemyManager.FixedUpdate(FixedDeltaTime);
}

void SCLGameManager::NotifyQuitPressed()
{
	m_pMain->NotifyQuit();
}

void SCLGameManager::NotifyEndGame()
{
	m_pMain->NotifyEnd();
}

void SCLGameManager::NotifyEnemyDeath(const SCLEnemy* pEnemy)
{
	// 5% chance to spawn pickup
	int Rand = SCLRandom::GetRandomInt(1, 20);
	if (Rand == 11)
	{
		m_MapManager.CreatePickups(pEnemy->GetPosition());
	}
	// Increase the score
	m_Score += pEnemy->GetScore();
}

glm::vec2 SCLGameManager::GetPlayerPosition() const
{
	return m_Player.GetPosition();
}

void SCLGameManager::MovePlayer(SCLMoveDir Dir)
{
	m_Player.AddMoveDir(Dir);
}

void SCLGameManager::StopPlayer(SCLMoveDir Dir)
{
	m_Player.RemoveMoveDir(Dir);
}

void SCLGameManager::RotatePlayerFiringDir(bool IsClockwise)
{
	m_Player.RotateFiringDir(IsClockwise);
}

void SCLGameManager::StopRotatePlayerFiringDir()
{
	m_Player.StopRotateFiringDir();
}

void SCLGameManager::SetPlayerAttackState(bool IsAttacking)
{
	m_Player.UpdateFiringState(IsAttacking);
}

void SCLGameManager::SetPlayerAttackPos(glm::vec2 AttackPos)
{
	m_Player.UpdateFiringPosition(AttackPos);
}

SCLGameObject* SCLGameManager::GetPlayerGameObject()
{
	return &m_Player;
}

SCLGrid* SCLGameManager::GetEnemyGrid()
{
	return m_EnemyManager.GetEnemyGrid();
}

SCLGrid* SCLGameManager::GetMapGrid()
{
	return m_MapManager.GetMapGrid();
}

const SCLCamera* SCLGameManager::GetCamera() const
{
	return m_pCamera;
}

void SCLGameManager::MoveCamera(glm::vec2 Displacement)
{
	m_pCamera->Translate(Displacement);
}

glm::vec2 SCLGameManager::GetWorldBoundsX() const
{
	float CamSizeX = m_pCamera->GetCameraSize().x;
	// 3x3 screens world:
	float HalfX = CamSizeX * 0.5f * SCLConstants::SCREEN_COUNT;
	return glm::vec2(-HalfX, HalfX);
}

glm::vec2 SCLGameManager::GetWorldBoundsY() const
{
	float CamSizeY = m_pCamera->GetCameraSize().y;
	// 3x3 screens world:
	float HalfY = CamSizeY * 0.5f * SCLConstants::SCREEN_COUNT;
	return glm::vec2(-HalfY, HalfY);
}

const std::vector<const SCLSprite*>* SCLGameManager::GetSprites() const
{
	// VisualManager will render these
	return &m_Sprites;
}

void SCLGameManager::AddSprite(const SCLSprite* pSprite)
{
	// Add only sprites that are seen
	if (m_pCamera->IsSpriteSeen(pSprite))
	{
		m_Sprites.push_back(pSprite);
	}
}

float SCLGameManager::GetFPS() const
{
	return m_pMain->GetFPS();
}

unsigned int SCLGameManager::GetScore() const
{
	return m_Score;
}

unsigned int SCLGameManager::GetPlayerHP() const
{
	return m_Player.GetHP();
}