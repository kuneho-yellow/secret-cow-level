#include "scl_enemy.h"
#include "scl_grid.h"
#include "scl_resource_manager.h"
#include "scl_game_constants.h"
#include "scl_random.h"

SCLEnemy::SCLEnemy()
	: m_Type(0),
	m_EnemyIndex(0),
	m_HP(0),
	m_Score(0),
	m_MoveDir(0.0f),
	m_MoveSpeed(0.0f),
	m_pEnemyGrid(NULL),
	m_pMapGrid(NULL),
	m_pPlayer(NULL)
{
	SetCollisionLayer(LAYER_ENEMY);
}

SCLEnemy::~SCLEnemy()
{
}

void SCLEnemy::Initialize(unsigned int Type, unsigned int EnemyIndex, glm::vec2 InitPos)
{
	m_Type = Type;
	m_EnemyIndex = EnemyIndex;
	m_MoveSpeed = GetMoveSpeedFromType(m_Type);
	m_HP = GetMaxHPFromType(m_Type);
	m_Score = GetScoreFromType(m_Type);
	SetSprite(GetTextureFromType(m_Type), SCLConstants::ENEMY_SIZE);
	SetColliderRadius(SCLConstants::ENEMY_COLLIDER_SIZE);
	SetPosition(InitPos);
	m_IsActive = true;
}

void SCLEnemy::Update(float DeltaTime)
{
	if (!m_IsActive)
	{
		return;
	}

	if (IsDead())
	{
		return;
	}

	// Update the movement
	if (m_pPlayer != NULL)
	{
		// Very simple movement: Just go after the player
		m_MoveDir = m_pPlayer->GetPosition() - GetPosition();
		m_MoveDir = glm::normalize(m_MoveDir);
		Move(m_MoveDir * m_MoveSpeed * DeltaTime);

		// Update it's position in the grid
		m_pEnemyGrid->UpdateObject(this);

		// Update the sprite's facing direction
		if (m_MoveDir.x > 0)
		{
			SetSpriteDirection(true);
		}
		else if (m_MoveDir.x < 0)
		{
			SetSpriteDirection(false);
		}
	}

}

void SCLEnemy::FixedUpdate(float FixedDeltaTime)
{
	if (!m_IsActive)
	{
		return;
	}

	CheckCollisionsWithMap(FixedDeltaTime);
	CheckCollisionsWithEnemies(FixedDeltaTime);
}

void SCLEnemy::NotifyCollision(SCLGameObject& OtherGO)
{
	switch (OtherGO.GetCollisionLayer())
	{
	case LAYER_BULLET:
		ProcessHitByBullet();
		break;

	case LAYER_ENEMY:
		// Move away from each other
		AdjustCollisionDepthHalf(OtherGO);
		OtherGO.AdjustCollisionDepthHalf(*this);
		break;

	case LAYER_PLAYER:
		// More lax with player
		AdjustCollisionDepthHalf(OtherGO);
		break;

	case LAYER_MAP:
		AdjustCollisionDepthFull(OtherGO);
		break;
	}
}

void SCLEnemy::SetEnemyGrid(SCLGrid* pEnemyGrid)
{
	m_pEnemyGrid = pEnemyGrid;
}

void SCLEnemy::SetMapGrid(SCLGrid* pMapGrid)
{
	m_pMapGrid = pMapGrid;
}

void SCLEnemy::SetPlayer(SCLGameObject* pPlayer)
{
	m_pPlayer = pPlayer;
}

bool SCLEnemy::IsDead() const
{
	return m_HP <= 0;
}

unsigned int SCLEnemy::GetScore() const
{
	return m_Score;
}

bool SCLEnemy::IsSpawnPickup() const
{
	return GetPickupChanceFromType(m_Type);
}

void SCLEnemy::CheckCollisionsWithMap(float FixedDeltaTime)
{
	if (m_pMapGrid == NULL)
	{
		return;
	}

	// Check for collisions with other enemies
	SCLCellCoord OwnCell = m_pMapGrid->GetCellCoord(GetPosition());
	// Check all neighbor cells
	SCLCellCoord CellCoord = OwnCell;
	for (CellCoord.x = OwnCell.x - 1; CellCoord.x <= OwnCell.x + 1; CellCoord.x += 1)
	{
		for (CellCoord.y = OwnCell.y - 1; CellCoord.y <= OwnCell.y + 1; CellCoord.y += 1)
		{
			SCLCell* ThisCell = m_pMapGrid->GetCell(CellCoord);
			if (ThisCell != NULL)
			{
				for (unsigned int ItemI = 0; ItemI < ThisCell->Objects.size(); ++ItemI)
				{
					SCLGameObject& MapItem = *(ThisCell->Objects[ItemI]);
					CheckCollision(MapItem);
				}
			}
		}
	}
}

void SCLEnemy::CheckCollisionsWithEnemies(float FixedDeltaTime)
{
	if (m_pEnemyGrid == NULL)
	{
		return;
	}

	// Check for collisions with other enemies
	SCLCellCoord OwnCell = m_pEnemyGrid->GetCellCoord(GetPosition());
	// Check neighbor cells in L-shape
	SCLCellCoord CellCoord = OwnCell;
	for (CellCoord.x = OwnCell.x - 1; CellCoord.x <= OwnCell.x; CellCoord.x += 1)
	{
		for (CellCoord.y = OwnCell.y - 1; CellCoord.y <= OwnCell.y + 1; CellCoord.y += 1)
		{
			SCLCell* ThisCell = m_pEnemyGrid->GetCell(CellCoord);
			if (ThisCell != NULL)
			{
				for (unsigned int EnemyI = 0; EnemyI < ThisCell->Objects.size(); ++EnemyI)
				{
					SCLGameObject& OtherEnemy = *(ThisCell->Objects[EnemyI]);
					if (&OtherEnemy != this)
					{
						CheckCollision(OtherEnemy);
					}
				}
			}
			if (CellCoord.x == OwnCell.x && CellCoord.y == OwnCell.y)
			{
				// Stop after checking your own cell
				break;
			}

		}
	}
}

void SCLEnemy::ProcessHitByBullet()
{
	if (m_HP > 0)
	{
		m_HP -= 1;
	}
}

// TODO: Do these properly somewhere else

unsigned int SCLEnemy::GetMaxHPFromType(unsigned int Type)
{
	switch (Type)
	{
	default:
	case 0:
		return 1;
	case 1:
		return 3;
	case 2:
		return 5;
	}
}

unsigned int SCLEnemy::GetScoreFromType(unsigned int Type)
{
	switch (Type)
	{
	default:
	case 0:
		return 10;
	case 1:
		return 25;
	case 2:
		return 50;
	}
}

float SCLEnemy::GetMoveSpeedFromType(unsigned int Type)
{
	switch (Type)
	{
	default:
	case 0:
		return 1.0f;
	case 1:
		return 1.0f;
	case 2:
		return 1.0f;
	}
}

SCLTexture SCLEnemy::GetTextureFromType(unsigned int Type)
{
	switch (Type)
	{
	default:
	case 0:
		return SCLResourceManager::GetLoadedTexture(SCLResources::TextureType_Cow1);
	case 1:
		return SCLResourceManager::GetLoadedTexture(SCLResources::TextureType_Cow2);
	case 2:
		return SCLResourceManager::GetLoadedTexture(SCLResources::TextureType_Cow3);
	}
}

bool SCLEnemy::GetPickupChanceFromType(unsigned int Type)
{
	// TODO: .....
	switch (Type)
	{
	default:
	case 0:
		return SCLRandom::GetRandomInt(0, 100) == 11;
	case 1:
		return SCLRandom::GetRandomInt(0, 50) == 11;
	case 2:
		return SCLRandom::GetRandomInt(0, 20) == 11;
	}
}