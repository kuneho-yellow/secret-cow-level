#include "scl_enemy_manager.h"
#include "scl_enemy.h"
#include "scl_game_manager.h"
#include "scl_game_constants.h"
#include "scl_camera.h"
#include "scl_random.h"

SCLEnemyManager::SCLEnemyManager()
	: m_pGameManager(NULL),
	m_pEnemies(NULL),
	m_EnemyCurrentIndex(0),
	m_EnemyGrid()
{
	// Create a large batch of enemies
	m_pEnemies = new SCLEnemy[SCLConstants::ENEMY_COUNT_MAX];
	if (m_pEnemies == NULL)
	{
		printf("ERROR: Enemies could not be created\n");
	}
}

SCLEnemyManager::~SCLEnemyManager()
{
	if (m_pEnemies != NULL)
	{
		delete[] m_pEnemies;
	}
}

void SCLEnemyManager::Initialize(SCLGameManager* pGameManager)
{
	m_pGameManager = pGameManager;
	
	// Initialize the enemy grid
	glm::vec2 CameraSize = m_pGameManager->GetCamera()->GetCameraSize();
	int CellSize = (int)ceil((CameraSize.x / SCLConstants::ENEMY_GRID_SCREEN_DIV));
	int Width = (int)ceil(CameraSize.x * (SCLConstants::SCREEN_COUNT));
	int Height = (int)ceil(CameraSize.y * (SCLConstants::SCREEN_COUNT));
	m_EnemyGrid.Initialize(Width, Height, CellSize);

	// Create enemies
	// Just spawn everyone at the start

	// Get the player's cell
	SCLCellCoord PlayerCell(-1, -1);
	if (m_pGameManager != NULL)
	{
		glm::vec2 PlayerPos = m_pGameManager->GetPlayerPosition();
		PlayerCell = m_EnemyGrid.GetCellCoord(PlayerPos);
	}

	// Create enemies in random cells
	while (m_EnemyCurrentIndex < SCLConstants::ENEMY_COUNT_MAX)
	{
		CreateNewEnemy(GetRandomSpawnableCell());
	}
}

void SCLEnemyManager::Update(float DeltaTime)
{
	// Update all enemies
	UpdateEnemies(DeltaTime);
}

void SCLEnemyManager::FixedUpdate(float FixedDeltaTime)
{
	// Update all enemies
	UpdateEnemyCollisions(FixedDeltaTime);
}

SCLGrid* SCLEnemyManager::GetEnemyGrid()
{
	return &m_EnemyGrid;
}

void SCLEnemyManager::CreateNewEnemy(SCLCellCoord CellCoord)
{
	if (m_pEnemies == NULL)
	{
		return;
	}

	// Start over to index 0 when the last slot in the vector has been reached
	if (m_EnemyCurrentIndex >= SCLConstants::ENEMY_COUNT_MAX)
	{
		m_EnemyCurrentIndex = 0;
	}

	// Initialize the enemy
	// Note: All first spawns are the weakest type
	InitializeEnemy(0, m_EnemyCurrentIndex, CellCoord);
	// Add the enemy to the enemy grid
	m_EnemyGrid.AddObject(&(m_pEnemies[m_EnemyCurrentIndex]));
	// Update the index for the next enemy
	m_EnemyCurrentIndex++;
}

void SCLEnemyManager::InitializeEnemy(unsigned int Type, unsigned int EnemyIndex, SCLCellCoord CellCoord)
{
	if (m_pEnemies == NULL)
	{
		return;
	}

	// TODO: Check for obstacles in spawn position
	glm::vec2 EnemyPos = GetRandomPositionInCell(CellCoord);
	// Initialize the enemy
	m_pEnemies[EnemyIndex].Initialize(Type, EnemyIndex, EnemyPos);
	m_pEnemies[EnemyIndex].SetEnemyGrid(&m_EnemyGrid);
	m_pEnemies[EnemyIndex].SetMapGrid(m_pGameManager->GetMapGrid());
	m_pEnemies[EnemyIndex].SetPlayer(m_pGameManager->GetPlayerGameObject());
}

void SCLEnemyManager::UpdateEnemies(float DeltaTime)
{
	if (m_pEnemies == NULL)
	{
		return;
	}

	for (unsigned int EnemyI = 0; EnemyI < SCLConstants::ENEMY_COUNT_MAX; ++EnemyI)
	{
		SCLEnemy& ThisEnemy = m_pEnemies[EnemyI];

		if (!ThisEnemy.IsActive())
		{
			continue;
		}

		// Update each enemy
		ThisEnemy.Update(DeltaTime);
		if (ThisEnemy.IsDead())
		{
			// If the enemy is dead, immediately replace it with a new one
			m_pGameManager->NotifyEnemyDeath(&ThisEnemy);
			//m_EnemyGrid.RemoveObject(&ThisEnemy); // No need for this
			SCLCellCoord RandomCell = GetRandomSpawnableCell();
			InitializeEnemy(GetEnemyTypeToSpawn(), EnemyI, RandomCell);
		}

		// Draw the enemy
		m_pGameManager->AddSprite(ThisEnemy.GetSprite());
	}
}

void SCLEnemyManager::UpdateEnemyCollisions(float FixedDeltaTime)
{
	for (unsigned int EnemyI = 0; EnemyI < SCLConstants::ENEMY_COUNT_MAX; ++EnemyI)
	{
		m_pEnemies[EnemyI].FixedUpdate(FixedDeltaTime);
	}
}

unsigned int SCLEnemyManager::GetEnemyTypeToSpawn()
{
	// Random for now
	return SCLRandom::GetRandomInt(0, SCLConstants::ENEMY_TYPE_MAX - 1);
}

SCLCellCoord SCLEnemyManager::GetRandomSpawnableCell() const
{
	// Get the player's cell
	SCLCellCoord PlayerCell(-1, -1);
	if (m_pGameManager != NULL)
	{
		glm::vec2 PlayerPos = m_pGameManager->GetPlayerPosition();
		PlayerCell = m_EnemyGrid.GetCellCoord(PlayerPos);
	}

	// TODO: So ugly...
	SCLCellCoord RandomCell(0, 0);
	int CellDistFromPlayer = SCLConstants::ENEMY_GRID_CELLS_FROM_PLAYER;
	RandomCell.x = SCLRandom::GetRandomInt(0, m_EnemyGrid.GetRowSize());
	RandomCell.y = SCLRandom::GetRandomInt(0, m_EnemyGrid.GetColumnSize());
	while ((int)RandomCell.x >= PlayerCell.x - CellDistFromPlayer &&
		(int)RandomCell.x <= PlayerCell.x + CellDistFromPlayer &&
		(int)RandomCell.y >= PlayerCell.y - CellDistFromPlayer &&
		(int)RandomCell.y <= PlayerCell.y + CellDistFromPlayer)
	{
		// TODO: Might never get out of here!
		RandomCell.x = SCLRandom::GetRandomInt(0, m_EnemyGrid.GetRowSize());
		RandomCell.y = SCLRandom::GetRandomInt(0, m_EnemyGrid.GetColumnSize());
	}
	return RandomCell;
}

glm::vec2 SCLEnemyManager::GetRandomPositionInCell(SCLCellCoord CellCoord) const
{
	glm::vec2 XBounds = m_EnemyGrid.GetCellBoundsX(CellCoord.x);
	glm::vec2 YBounds = m_EnemyGrid.GetCellBoundsY(CellCoord.y);
	glm::vec2 RandomPos(0.0f, 0.0f);
	RandomPos.x = SCLRandom::GetRandomFloat(XBounds.x, XBounds.y);
	RandomPos.y = SCLRandom::GetRandomFloat(YBounds.x, YBounds.y);
	return RandomPos;
}
