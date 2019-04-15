#pragma once

#include "scl_grid.h"

class SCLGameManager;
class SCLEnemy;

class SCLEnemyManager
{
public:
	SCLEnemyManager();
	~SCLEnemyManager();

	void Initialize(SCLGameManager* pGameManager);
	void Update(float DeltaTime);
	void FixedUpdate(float FixedDeltaTime);

	// For checking collisions
	SCLGrid* GetEnemyGrid();

private:
	void CreateNewEnemy(SCLCellCoord CellCoord);
	void InitializeEnemy(unsigned int Type, unsigned int EnemyIndex, SCLCellCoord CellCoord);
	void UpdateEnemies(float DeltaTime);
	void UpdateEnemyCollisions(float FixedDeltaTime);
	unsigned int GetEnemyTypeToSpawn();
	SCLCellCoord GetRandomSpawnableCell() const;
	glm::vec2 GetRandomPositionInCell(SCLCellCoord CellCoord) const;

	SCLGameManager* m_pGameManager;
	SCLEnemy* m_pEnemies;
	unsigned int m_EnemyCurrentIndex;
	SCLGrid m_EnemyGrid;
};

