#include "scl_bullet.h"
#include "scl_grid.h"

SCLBullet::SCLBullet()
	: m_BulletIndex(0),
	m_MoveDir(0.0f),
	m_MoveSpeed(0.0f),
	m_MoveDistMax(0.0f),
	m_MoveTimeMax(0.0f),
	m_MoveTimer(0.0f),
	m_pEnemyGrid(NULL),
	m_pMapGrid(NULL)
{
	SetCollisionLayer(LAYER_BULLET);
}


SCLBullet::~SCLBullet()
{
	
}

void SCLBullet::Initialize(SCLTexture Texture, float Size, unsigned int BulletIndex,
				glm::vec2 InitPos, glm::vec2 MoveDir, float MoveSpeed, float DistMax)
{
	m_BulletIndex = BulletIndex;
	SetSprite(Texture, Size);
	SetPosition(InitPos);
	m_MoveDir = MoveDir;
	m_MoveSpeed = MoveSpeed;
	m_MoveDistMax = DistMax;
	m_MoveTimeMax = (m_MoveSpeed != 0.0f) ? m_MoveDistMax / m_MoveSpeed : 0.0f;
	m_MoveTimer = 0.0f;
	SetColliderRadius(Size * 0.5f);
	m_IsActive = true;
}

void SCLBullet::Update(float DeltaTime)
{
	if (!m_IsActive)
	{
		return;
	}

	m_MoveTimer += DeltaTime;
	if (m_MoveTimer >= m_MoveTimeMax)
	{
		m_MoveTimer = m_MoveTimeMax;
		m_IsActive = false;
		return;
	}
	Move(m_MoveDir * m_MoveSpeed * DeltaTime);
}

void SCLBullet::FixedUpdate(float FixedDeltaTime)
{
	if (!m_IsActive)
	{
		return;
	}
	
	CheckCollisionsWithMap(FixedDeltaTime);
	CheckCollisionsWithEnemies(FixedDeltaTime);
}

void SCLBullet::NotifyCollision(SCLGameObject& OtherGO)
{
	if (!m_IsActive)
	{
		return;
	}
	
	switch (OtherGO.GetCollisionLayer())
	{
	case SCLCollisionLayer::LAYER_ENEMY:
		OtherGO.NotifyCollision(*this);
		m_IsActive = false;
		break;

	case SCLCollisionLayer::LAYER_MAP:
		m_IsActive = false;
		break;
	}
}

void SCLBullet::SetEnemyGrid(SCLGrid* pEnemyGrid)
{
	m_pEnemyGrid = pEnemyGrid;
}

void SCLBullet::SetMapGrid(SCLGrid* pMapGrid)
{
	m_pMapGrid = pMapGrid;
}

void SCLBullet::CheckCollisionsWithMap(float FixedDeltatTime)
{
	if (!m_IsActive)
	{
		return;
	}

	if (m_pMapGrid == NULL)
	{
		return;
	}

	// Check for collisions with map items
	SCLCellCoord BulletCell = m_pMapGrid->GetCellCoord(GetPosition());
	// Check neighbor cells
	SCLCellCoord CellCoord = BulletCell;
	for (CellCoord.x = BulletCell.x - 1; CellCoord.x <= BulletCell.x + 1; CellCoord.x += 1)
	{
		for (CellCoord.y = BulletCell.y - 1; CellCoord.y <= BulletCell.y + 1; CellCoord.y += 1)
		{
			SCLCell* ThisCell = m_pMapGrid->GetCell(CellCoord);
			if (ThisCell != NULL)
			{
				for (unsigned int ItemI = 0; ItemI < ThisCell->Objects.size(); ++ItemI)
				{
					SCLGameObject& ThisItem = *(ThisCell->Objects[ItemI]);
					CheckCollision(ThisItem);
				}
			}
		}
	}
}

void SCLBullet::CheckCollisionsWithEnemies(float FixedDeltatTime)
{
	if (!m_IsActive)
	{
		return;
	}

	if (m_pEnemyGrid == NULL)
	{
		return;
	}

	// Check for collisions with enemies
	SCLCellCoord BulletCell = m_pEnemyGrid->GetCellCoord(GetPosition());
	// Check neighbor cells
	SCLCellCoord CellCoord = BulletCell;
	for (CellCoord.x = BulletCell.x - 1; CellCoord.x <= BulletCell.x + 1; CellCoord.x += 1)
	{
		for (CellCoord.y = BulletCell.y - 1; CellCoord.y <= BulletCell.y + 1; CellCoord.y += 1)
		{
			SCLCell* ThisCell = m_pEnemyGrid->GetCell(CellCoord);
			if (ThisCell != NULL)
			{
				for (unsigned int EnemyI = 0; EnemyI < ThisCell->Objects.size(); ++EnemyI)
				{
					SCLGameObject& ThisEnemy = *(ThisCell->Objects[EnemyI]);
					CheckCollision(ThisEnemy);
				}
			}
		}
	}
}