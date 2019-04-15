#pragma once

#include "scl_game_object.h"
#include "scl_map_item.h"
#include "scl_grid.h"

class SCLGameManager;

class SCLMapManager
{
public:
	SCLMapManager();
	~SCLMapManager();

	void Initialize(SCLGameManager* pGameManager);
	void Update(float DeltaTime);
	void FixedUpdate(float FixedDeltaTime);

	// For collision detection
	SCLGrid* GetMapGrid();

	void CreatePickups(glm::vec2 Position);

private:
	void CreateItem(SCLCellCoord CellCoord);
	void CreateCollidableItem(SCLCellCoord CellCoord);
	void CreatePickups(SCLCellCoord CellCoord);
	SCLMapItemType GetRandomPickupType();
	glm::vec2 GetRandomPositionInCell(SCLCellCoord CellCoord) const;

	SCLGameManager* m_pGameManager;
	SCLMapItem* m_pBG;
	SCLMapItem* m_pMapItems;
	SCLMapItem* m_pCollidableMapItems;
	SCLMapItem* m_pPickups;
	unsigned int m_ItemIndex;
	unsigned int m_ItemCount;
	unsigned int m_CollidableItemIndex;
	unsigned int m_CollidableItemCount;
	unsigned int m_PickupsIndex;
	SCLGrid m_MapGrid;
};

