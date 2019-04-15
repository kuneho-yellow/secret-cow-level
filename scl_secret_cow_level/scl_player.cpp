#include "scl_player.h"
#include "scl_game_manager.h"
#include "scl_game_constants.h"
#include "scl_resource_manager.h"
#include "scl_camera.h"
#include "scl_bullet.h"
#include "scl_random.h"

SCLPlayer::SCLPlayer()
	: m_pGameManager(NULL),
	m_pBullets(NULL),
	m_BulletCurrentIndex(0),
	m_MoveDir(0.0f, 0.0f),
	m_FiringPos(0.0f),
	m_FiringTimer(0.0f),
	m_FiringSpeed(SCLConstants::PLAYER_FIRING_SPEED_MIN),
	m_FiringConeAngle(SCLConstants::PLAYER_FIRING_ANGLE_MIN),
	m_IsFiring(false),
	m_RotateState(0),
	m_IsSuperMode(false),
	m_SuperModeTimer(0.0f),
	m_SuperModeFiringSpeed(SCLConstants::PLAYER_SUPERMODE_SPEED),
	m_SuperModeFiringConeAngle(SCLConstants::PLAYER_SUPERMODE_ANGLE),
	m_HP(0),
	m_IsInvincible(false),
	m_InvincibleTimer(0.0f)
{
	for (unsigned int DirI = 0; DirI < MOVE_DIR_SIZE; ++DirI)
	{
		m_MoveStates[DirI] = false;
	}

	// Create a large batch of bullets
	m_pBullets = new SCLBullet[SCLConstants::BULLET_COUNT_MAX];
	if (m_pBullets == NULL)
	{
		printf("ERROR: Player bullets could not be created\n");
	}

	SetCollisionLayer(LAYER_PLAYER);
}

SCLPlayer::~SCLPlayer()
{
	if (m_pBullets != NULL)
	{
		delete[] m_pBullets;
	}
}

void SCLPlayer::Initialize(SCLGameManager* pGameManager)
{
	m_pGameManager = pGameManager;
	SetSprite(SCLResourceManager::GetLoadedTexture(SCLResources::TextureType_Player), SCLConstants::PLAYER_SIZE);
	SetColliderRadius(SCLConstants::PLAYER_SIZE * 0.5f);
	m_HP = SCLConstants::PLAYER_HP_MAX;
	m_IsActive = true;
}

void SCLPlayer::Update(float DeltaTime)
{
	// Update player movement
	if (UpdateMoveDir())
	{
		// Adjust the player's facing direction according to its movement
		if (m_MoveDir.x > 0)
		{
			SetSpriteDirection(true);
		}
		else if (m_MoveDir.x < 0)
		{
			SetSpriteDirection(false);
		}

		glm::vec2 OldPos = GetPosition();

		// Move the player
		glm::vec2 MoveVec = m_MoveDir * DeltaTime * SCLConstants::PLAYER_MOVE_SPEED;
		Move(MoveVec);

		// Don't go off the world's edges
		bool HasPosAdjusted = false;
		glm::vec2 WorldBoundsX = m_pGameManager->GetWorldBoundsX();
		glm::vec2 WorldBoundsY = m_pGameManager->GetWorldBoundsY();
		glm::vec2 NewPos = GetPosition();
		if (NewPos.x < WorldBoundsX.x)
		{
			HasPosAdjusted = true;
			NewPos.x = WorldBoundsX.x;
		}
		else if (NewPos.x > WorldBoundsX.y)
		{
			HasPosAdjusted = true;
			NewPos.x = WorldBoundsX.y;
		}
		if (NewPos.y < WorldBoundsY.x)
		{
			HasPosAdjusted = true;
			NewPos.y = WorldBoundsY.x;
		}
		else if (NewPos.y > WorldBoundsY.y)
		{
			HasPosAdjusted = true;
			NewPos.y = WorldBoundsY.y;
		}
		if (HasPosAdjusted)
		{
			SetPosition(NewPos);
			MoveVec = NewPos - OldPos; // Actual movement vector
		}

		glm::vec2 ViewportPos = m_pGameManager->GetCamera()->ConvertWorldToViewport(GetPosition());

		// Scroll the camera if needed
		if ((ViewportPos.x > SCLConstants::EDGE_SCROLLING_MAX && MoveVec.x > 0.0f) ||
			(ViewportPos.x < SCLConstants::EDGE_SCROLLING_MIN && MoveVec.x < 0.0f))
		{
			m_pGameManager->MoveCamera(glm::vec2(MoveVec.x, 0.0f));
		}
		if ((ViewportPos.y > SCLConstants::EDGE_SCROLLING_MAX && MoveVec.y > 0.0f) ||
			(ViewportPos.y < SCLConstants::EDGE_SCROLLING_MIN && MoveVec.y < 0.0f))
		{
			m_pGameManager->MoveCamera(glm::vec2(0.0f, MoveVec.y));
		}
	}

	UpdateFiringDirRotation(DeltaTime);
	// Update the player's attack
	if (m_IsFiring || SCLConstants::INPUT_IS_AUTOFIRE)
	{
		// Fire bullets at a certain frequency
		m_FiringTimer += DeltaTime;
		float TimePerBullet = 1 / m_FiringSpeed;
		while (m_FiringTimer > TimePerBullet)
		{
			CreateBullet();
			m_FiringTimer -= TimePerBullet;
		}

		// Adjust the player's facing direction according to its firing direction
		if (m_FiringPos.x > GetPosition().x)
		{
			SetSpriteDirection(true);
		}
		else if (m_FiringPos.x < GetPosition().x)
		{
			SetSpriteDirection(false);
		}
	}

	if (m_IsInvincible)
	{
		m_InvincibleTimer += DeltaTime;
		if (m_InvincibleTimer >= SCLConstants::PLAYER_INVINCIBLE_DUR)
		{
			EndInvincibility();
		}
	}

	if (m_IsSuperMode)
	{
		m_SuperModeTimer += DeltaTime;
		if (m_SuperModeTimer >= SCLConstants::PLAYER_SUPERMODE_DUR)
		{
			EndSuperMode();
		}
	}

	// Update all bullets
	UpdateBullets(DeltaTime);

	// Draw the player
	m_pGameManager->AddSprite(GetSprite());
}

void SCLPlayer::FixedUpdate(float FixedDeltaTime)
{
	/// Update collisions with map
	CheckCollisionsWithMap(FixedDeltaTime);

	// Update collisions with enemies
	CheckCollisionsWithEnemies(FixedDeltaTime);

	// Update bullet collisions
	UpdateBulletCollisions(FixedDeltaTime);
}

void SCLPlayer::NotifyCollision(SCLGameObject& OtherGO)
{
	if (!m_IsActive)
	{
		return;
	}

	switch (OtherGO.GetCollisionLayer())
	{
	case LAYER_ENEMY:
		ProcessHitByEnemy();
		AdjustCollisionDepthHalf(OtherGO);
		OtherGO.NotifyCollision(*this);
		break;

	case LAYER_MAP:
		AdjustCollisionDepthFull(OtherGO);
		break;

	case LAYER_PICKUP:
		ProcessGetPickup(OtherGO);
		break;

	default:
		break;
	}
}

unsigned int SCLPlayer::GetHP() const
{
	return m_HP;
}

void SCLPlayer::AddMoveDir(SCLMoveDir Dir)
{
	m_MoveStates[Dir] = true;

	switch (Dir)
	{
	case MOVE_DIR_LEFT:
		m_MoveStates[MOVE_DIR_RIGHT] = false;
		break;
	case MOVE_DIR_RIGHT:
		m_MoveStates[MOVE_DIR_LEFT] = false;
		break;
	case MOVE_DIR_UP:
		m_MoveStates[MOVE_DIR_DOWN] = false;
		break;
	case MOVE_DIR_DOWN:
		m_MoveStates[MOVE_DIR_UP] = false;
		break;
	default:
		break;
	}
}

void SCLPlayer::RemoveMoveDir(SCLMoveDir Dir)
{
	m_MoveStates[Dir] = false;
}

void SCLPlayer::UpdateFiringState(bool IsFiring)
{
	m_IsFiring = IsFiring;
}

void SCLPlayer::UpdateFiringPosition(glm::vec2 FiringPos)
{
	m_FiringPos = FiringPos;
	m_FiringDir = m_FiringPos - GetPosition();
	// Need to normalize it
	m_FiringDir = glm::normalize(m_FiringDir);
	// Prioritize this over rotation
	m_RotateState = 0;
}

void SCLPlayer::RotateFiringDir(bool IsClockwise)
{
	m_RotateState = IsClockwise ? 1 : -1;
}

void SCLPlayer::StopRotateFiringDir()
{
	m_RotateState =  0;
}

bool SCLPlayer::UpdateMoveDir()
{
	// Update Move Direction
	bool IsMoving = false;
	m_MoveDir = glm::vec2(0.0f);
	if (m_MoveStates[MOVE_DIR_LEFT])
	{
		m_MoveDir.x = -1.0f;
		IsMoving = true;
	}
	if (m_MoveStates[MOVE_DIR_RIGHT])
	{
		m_MoveDir.x = 1.0f;
		IsMoving = true;
	}
	if (m_MoveStates[MOVE_DIR_UP])
	{
		m_MoveDir.y = 1.0f;
		IsMoving = true;
	}
	if (m_MoveStates[MOVE_DIR_DOWN])
	{
		m_MoveDir.y = -1.0f;
		IsMoving = true;
	}

	if (IsMoving)
	{
		// Need to nrmalize it...
		m_MoveDir = glm::normalize(m_MoveDir);
		return true;
	}
	return false;
}

void SCLPlayer::CreateBullet()
{
	if (m_pBullets == NULL)
	{
		return;
	}

	// Reuse the first instance when you hit the max no. of bullets
	if (m_BulletCurrentIndex >= SCLConstants::BULLET_COUNT_MAX)
	{
		m_BulletCurrentIndex = 0;
	}

	// Get the bullet instance already in the current slot
	SCLBullet& NewBullet = m_pBullets[m_BulletCurrentIndex];

	// Get the actual firing dir randomized depending on firing angle
	glm::vec2 ActualFiringDir = GetRandomFiringDir();

	// Create the bullet
	// TODO: Might want to make different bullet types
	glm::vec2 BulletPos = GetPosition() + (ActualFiringDir * SCLConstants::PLAYER_FIRING_DISTANCE);
	NewBullet.Initialize(SCLResourceManager::GetLoadedTexture(SCLResources::TextureType_Bullet1),
		SCLConstants::BULLET_SIZE, m_BulletCurrentIndex, BulletPos,
		ActualFiringDir, SCLConstants::BULLET_SPEED, SCLConstants::BULLET_DIST_MAX);
	// Pass reference to the grids for collision checking
	NewBullet.SetEnemyGrid(m_pGameManager->GetEnemyGrid());
	NewBullet.SetMapGrid(m_pGameManager->GetMapGrid());

	// Update the index for the next bullet
	m_BulletCurrentIndex++;
}

void SCLPlayer::UpdateBullets(float DeltaTime)
{
	if (m_pBullets == NULL)
	{
		return;
	}

	for (unsigned int BulletI = 0; BulletI < SCLConstants::BULLET_COUNT_MAX; ++BulletI)
	{
		// Update each bullet
		m_pBullets[BulletI].Update(DeltaTime);
		if (m_pBullets[BulletI].IsActive())
		{
			// Draw the bullet if it's active
			m_pGameManager->AddSprite(m_pBullets[BulletI].GetSprite());
		}
	}
}

void SCLPlayer::UpdateBulletCollisions(float FixedDeltaTime)
{
	if (m_pBullets == NULL)
	{
		return;
	}

	for (unsigned int BulletI = 0; BulletI < SCLConstants::BULLET_COUNT_MAX; ++BulletI)
	{
		// Update each bullet
		m_pBullets[BulletI].FixedUpdate(FixedDeltaTime);
	}
}

glm::vec2 SCLPlayer::GetRandomFiringDir()
{
	glm::vec2 RandomDir = m_FiringDir;
	if (!SCLConstants::INPUT_IS_ROTATE_ON)
	{
		// Get the firing direction (relative to player's position)
		RandomDir = m_FiringPos - GetPosition();
		RandomDir = glm::normalize(RandomDir);
	}
	// Randomize the direction a bit~
	float FiringRadians = atan2(RandomDir.y, RandomDir.x);
	float DeltaAngle = SCLRandom::GetRandomFloat(-m_FiringConeAngle, m_FiringConeAngle);
	FiringRadians += DeltaAngle * 3.1416f / 360.0f;
	RandomDir.x = cos(FiringRadians);
	RandomDir.y = sin(FiringRadians);

	return RandomDir;
}

void SCLPlayer::UpdateFiringDirRotation(float DeltaTime)
{
	if (m_RotateState == 0)
	{
		return;
	}

	// Rotate the firing direction at a certain speed
	float FiringRadians = atan2(m_FiringDir.y, m_FiringDir.x);
	float DeltaAngle = SCLConstants::INPUT_ROTATE_SPEED * DeltaTime;
	if (m_RotateState == 1)
	{
		DeltaAngle = -DeltaAngle;
	}
	FiringRadians += DeltaAngle * 3.1416f / 360.0f;
	m_FiringDir.x = cos(FiringRadians);
	m_FiringDir.y = sin(FiringRadians);
}

void SCLPlayer::CheckCollisionsWithEnemies(float FixedDeltaTime)
{
	if (!m_IsActive)
	{
		return;
	}

	SCLGrid* pEnemyGrid = m_pGameManager->GetEnemyGrid();
	if (pEnemyGrid == NULL)
	{
		return;
	}

	// Check for collisions with enemies
	SCLCellCoord OwnCell = pEnemyGrid->GetCellCoord(GetPosition());
	// Check neighbor cells
	SCLCellCoord CellCoord = OwnCell;
	for (CellCoord.x = OwnCell.x - 1; CellCoord.x <= OwnCell.x + 1; CellCoord.x += 1)
	{
		for (CellCoord.y = OwnCell.y - 1; CellCoord.y <= OwnCell.y + 1; CellCoord.y += 1)
		{
			SCLCell* ThisCell = pEnemyGrid->GetCell(CellCoord);
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

void SCLPlayer::ProcessHitByEnemy()
{
	if (m_IsInvincible)
	{
		return;
	}

	// Decrease Life Points
	if (m_HP > 0)
	{
		m_HP -= 1;
		if (m_HP <= 0)
		{
			ProcessDeath();
			m_HP = 0;
		}
		else
		{
			StartInvincibility();
		}
	}
}

void SCLPlayer::ProcessGetPickup(SCLGameObject& Pickup)
{
	SCLMapItem* pPickup = dynamic_cast<SCLMapItem*>(&Pickup);
	switch (pPickup->GetType())
	{
	case MAP_ITEM_PICKUP_1:
		m_HP += SCLConstants::PICKUP_HP_PLUS;
		if (m_HP > SCLConstants::PLAYER_HP_MAX)
		{
			m_HP = SCLConstants::PLAYER_HP_MAX;
		}
		break;
	case MAP_ITEM_PICKUP_2:
		m_FiringConeAngle += SCLConstants::PICKUP_ANGLE_PLUS;
		if (m_FiringConeAngle > SCLConstants::PLAYER_FIRING_ANGLE_MAX)
		{
			m_FiringConeAngle = SCLConstants::PLAYER_FIRING_ANGLE_MAX;
		}
		break;
	case MAP_ITEM_PICKUP_3:
		m_FiringSpeed += SCLConstants::PICKUP_SPEED_PLUS;
		if (m_FiringSpeed > SCLConstants::PLAYER_FIRING_SPEED_MAX)
		{
			m_FiringSpeed = SCLConstants::PLAYER_FIRING_SPEED_MAX;
		}
		break;
	case MAP_ITEM_PICKUP_4:
		StartSuperMode();
		break;
	default:
		break;
	}
}

void SCLPlayer::ProcessDeath()
{
	SetSprite(SCLResourceManager::GetLoadedTexture(SCLResources::TextureType_PlayerDead), SCLConstants::PLAYER_SIZE);
	m_pGameManager->NotifyEndGame();
}

void SCLPlayer::StartInvincibility()
{
	m_IsInvincible = true;
	m_InvincibleTimer = 0.0f;
	SetSpriteAlpha(SCLConstants::PLAYER_INVINCIBLE_ALPHA);
}

void SCLPlayer::EndInvincibility()
{
	m_IsInvincible = false;
	SetSpriteAlpha(1.0f);
}

void SCLPlayer::StartSuperMode()
{                          
	if (!m_IsSuperMode)
	{
		// Adjust firing speed and angle to super mode values
		float Temp = m_FiringSpeed;
		m_FiringSpeed = m_SuperModeFiringSpeed;
		m_SuperModeFiringSpeed = Temp;

		Temp = m_FiringConeAngle;
		m_FiringConeAngle = m_SuperModeFiringConeAngle;
		m_SuperModeFiringConeAngle = Temp;
	}

	m_IsSuperMode = true;
	m_SuperModeTimer = 0.0f;
}

void SCLPlayer::EndSuperMode()
{
	if (m_IsSuperMode)
	{
		// Bring back normal firing speed and angle
		float Temp = m_FiringSpeed;
		m_FiringSpeed = m_SuperModeFiringSpeed;
		m_SuperModeFiringSpeed = Temp;

		Temp = m_FiringConeAngle;
		m_FiringConeAngle = m_SuperModeFiringConeAngle;
		m_SuperModeFiringConeAngle = Temp;
	}

	m_IsSuperMode = false;
}

void SCLPlayer::CheckCollisionsWithMap(float FixedDeltaTime)
{
	if (!m_IsActive)
	{
		return;
	}

	SCLGrid* pMapGrid = m_pGameManager->GetMapGrid();
	if (pMapGrid == NULL)
	{
		return;
	}

	// Check for collisions with enemies
	SCLCellCoord OwnCell = pMapGrid->GetCellCoord(GetPosition());
	// Check neighbor cells
	SCLCellCoord CellCoord = OwnCell;
	for (CellCoord.x = OwnCell.x - 1; CellCoord.x <= OwnCell.x + 1; CellCoord.x += 1)
	{
		for (CellCoord.y = OwnCell.y - 1; CellCoord.y <= OwnCell.y + 1; CellCoord.y += 1)
		{
			SCLCell* ThisCell = pMapGrid->GetCell(CellCoord);
			if (ThisCell != NULL)
			{
				for (unsigned int MapI = 0; MapI < ThisCell->Objects.size(); ++MapI)
				{
					SCLGameObject& ThisItem = *(ThisCell->Objects[MapI]);
					CheckCollision(ThisItem);
				}
			}
		}
	}
}