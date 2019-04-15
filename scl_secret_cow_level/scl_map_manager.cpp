#include "scl_map_manager.h"
#include "scl_map_item.h"
#include "scl_game_manager.h"
#include "scl_game_constants.h"
#include "scl_camera.h"
#include "scl_resource_manager.h"
#include "scl_random.h"

SCLMapManager::SCLMapManager()
	: m_pGameManager(NULL),
	m_pBG(NULL),
	m_pMapItems(NULL),
	m_pCollidableMapItems(NULL),
	m_pPickups(NULL),
	m_ItemIndex(0),
	m_ItemCount(0),
	m_PickupsIndex(0),
	m_CollidableItemIndex(0),
	m_CollidableItemCount(0),
	m_MapGrid()
{
	m_pBG = new SCLMapItem();
	m_pPickups = new SCLMapItem[SCLConstants::PICKUP_COUNT_MAX];
}


SCLMapManager::~SCLMapManager()
{
	if (m_pBG != NULL)
	{
		delete m_pBG;
	}
	if (m_pMapItems != NULL)
	{
		delete[] m_pMapItems;
	}
	if (m_pCollidableMapItems != NULL)
	{
		delete[] m_pCollidableMapItems;
	}
	if (m_pPickups != NULL)
	{
		delete[] m_pPickups;
	}
}

void SCLMapManager::Initialize(SCLGameManager* pGameManager)
{
	m_pGameManager = pGameManager;

	// Initialize the background
	if (m_pBG != NULL)
	{
		float BGSize = m_pGameManager->GetCamera()->GetCameraSize().x * SCLConstants::SCREEN_COUNT;
		m_pBG->SetSprite(SCLResourceManager::GetLoadedTexture(SCLResources::TextureType_BG), BGSize);
		m_pBG->Initialize(MAP_ITEM_BG, 0, glm::vec2(0.0f, 0.0f));
	}
	
	// Initialize the map grid
	glm::vec2 CameraSize = m_pGameManager->GetCamera()->GetCameraSize();
	int Width = (int)ceil(CameraSize.x * SCLConstants::SCREEN_COUNT);
	int Height = (int)ceil(CameraSize.y * SCLConstants::SCREEN_COUNT);
	int CellSize = Width / SCLConstants::MAP_GRID_SCREEN_DIV;
	m_MapGrid.Initialize(Width, Height, CellSize);

	// Get the player's cell
	SCLCellCoord PlayerCell(-1, -1);
	if (m_pGameManager != NULL)
	{
		glm::vec2 PlayerPos = m_pGameManager->GetPlayerPosition();
		PlayerCell = m_MapGrid.GetCellCoord(PlayerPos);
	}

	// Create items in each cell
	int GridRowSize = m_MapGrid.GetRowSize();
	int GridColSize = m_MapGrid.GetColumnSize();
	int CellCount = (GridRowSize * GridColSize);
	m_ItemCount = CellCount * SCLConstants::MAP_DECOR_PER_CELL;
	m_CollidableItemCount = CellCount * SCLConstants::MAP_STATIC_ITEM_PER_CELL - 1;
	m_pMapItems = new SCLMapItem[m_ItemCount];
	m_pCollidableMapItems = new SCLMapItem[m_CollidableItemCount];
	for (int CellX = 0; CellX < GridRowSize; ++CellX)
	{
		for (int CellY = 0; CellY < GridColSize; ++CellY)
		{
			for (int ItemI = 0; ItemI < SCLConstants::MAP_DECOR_PER_CELL; ++ItemI)
			{
				CreateItem(SCLCellCoord(CellX, CellY));
			}
			// No collidable items in player's cell
			if (CellX >= PlayerCell.x && CellX <= PlayerCell.x &&
				CellY >= PlayerCell.y  && CellY <= PlayerCell.y )
			{
				continue;
			}
			for (int ItemI = 0; ItemI < SCLConstants::MAP_STATIC_ITEM_PER_CELL; ++ItemI)
			{
				CreateCollidableItem(SCLCellCoord(CellX, CellY));
			}
		}
	}
}

void SCLMapManager::Update(float DeltaTime)
{
	// Draw all the map items
	if (m_pBG != NULL)
	{
		m_pGameManager->AddSprite(m_pBG->GetSprite());
	}
	if (m_pMapItems != NULL)
	{
		for (unsigned int ItemI = 0; ItemI < m_ItemCount; ++ItemI)
		{
			if (m_pMapItems[ItemI].IsActive())
			{
				m_pGameManager->AddSprite(m_pMapItems[ItemI].GetSprite());
			}
		}
	}
	if (m_pCollidableMapItems != NULL)
	{
		for (unsigned int ItemI = 0; ItemI < m_CollidableItemCount; ++ItemI)
		{
			if (m_pCollidableMapItems[ItemI].IsActive())
			{
				m_pGameManager->AddSprite(m_pCollidableMapItems[ItemI].GetSprite());
			}
		}
	}
	if (m_pPickups != NULL)
	{
		for (unsigned int ItemI = 0; ItemI < SCLConstants::PICKUP_COUNT_MAX; ++ItemI)
		{
			if (m_pPickups[ItemI].IsActive())
			{
				m_pGameManager->AddSprite(m_pPickups[ItemI].GetSprite());
			}
		}
	}
}

void SCLMapManager::FixedUpdate(float FixedDeltaTime)
{
	// Collision detection for pickups
	if (m_pPickups != NULL)
	{
		for (unsigned int ItemI = 0; ItemI < SCLConstants::PICKUP_COUNT_MAX; ++ItemI)
		{
			m_pPickups[ItemI].FixedUpdate(FixedDeltaTime);
		}
	}
}

SCLGrid* SCLMapManager::GetMapGrid()
{
	return &m_MapGrid;
}

void SCLMapManager::CreatePickups(glm::vec2 Position)
{
	// Reuse the first instance when you run out
	if (m_PickupsIndex >= SCLConstants::PICKUP_COUNT_MAX)
	{
		m_PickupsIndex = 0;
	}

	// Get the instance already in the current slot
	SCLMapItem& NewPickup = m_pPickups[m_PickupsIndex];

	m_pPickups[m_PickupsIndex].Initialize(GetRandomPickupType(), m_PickupsIndex, Position);
	m_pPickups[m_PickupsIndex].SetPlayerGameObject(m_pGameManager->GetPlayerGameObject());
	m_PickupsIndex++;
}

void SCLMapManager::CreateItem(SCLCellCoord CellCoord)
{
	if (m_pMapItems == NULL)
	{
		return;
	}

	m_pMapItems[m_ItemIndex].Initialize(MAP_ITEM_DECOR,
										m_ItemIndex,
										GetRandomPositionInCell(CellCoord));
	m_ItemIndex++;
}
void SCLMapManager::CreateCollidableItem(SCLCellCoord CellCoord)
{
	if (m_pCollidableMapItems == NULL)
	{
		return;
	}

	m_pCollidableMapItems[m_CollidableItemIndex].Initialize(MAP_ITEM_STATIC,
															m_CollidableItemIndex,
															GetRandomPositionInCell(CellCoord));
	m_MapGrid.AddObject(&m_pCollidableMapItems[m_CollidableItemIndex]);
	m_CollidableItemIndex++;
}

SCLMapItemType SCLMapManager::GetRandomPickupType()
{
	int Rand = SCLRandom::GetRandomInt(MAP_ITEM_PICKUP_START + 1, MAP_ITEM_PICKUP_END - 1);
	return (SCLMapItemType)Rand;
}

glm::vec2 SCLMapManager::GetRandomPositionInCell(SCLCellCoord CellCoord) const
{
	glm::vec2 XBounds = m_MapGrid.GetCellBoundsX(CellCoord.x);
	glm::vec2 YBounds = m_MapGrid.GetCellBoundsY(CellCoord.y);
	glm::vec2 RandomPos(0.0f, 0.0f);
	RandomPos.x = SCLRandom::GetRandomFloat(XBounds.x, XBounds.y);
	RandomPos.y = SCLRandom::GetRandomFloat(YBounds.x, YBounds.y);
	return RandomPos;
}