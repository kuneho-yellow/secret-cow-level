#pragma once

#include <vector>
#include <glm/glm.hpp>

class SCLGameObject;

struct SCLCell
{
	std::vector<SCLGameObject*> Objects;
};

struct SCLCellCoord
{
	int x;
	int y;

	SCLCellCoord(int X, int Y)
	{
		x = X;
		y = Y;
	}
};

class SCLGrid
{
public:
	SCLGrid();
	~SCLGrid();

	void Initialize(int Width, int Height, int CellSize);
	void AddObject(SCLGameObject* ObjectToAdd);
	void UpdateObject(SCLGameObject* ObjectToUpdate);
	void RemoveObject(SCLGameObject* ObjectToRemove);
	SCLCell* GetCell(SCLCellCoord);
	SCLCell* GetCell(glm::vec2 Position);
	SCLCellCoord GetCellCoord(glm::vec2 Position) const;
	glm::vec2 GetCellBoundsX(int CellX) const;
	glm::vec2 GetCellBoundsY(int CellY) const;
	int GetRowSize() const;
	int GetColumnSize() const;

	std::vector<SCLCell> Cells;

private:
	int m_Width;
	int m_Height;
	int m_CellSize;
	int m_CellsInRowCount;
	int m_CellsInColCount;
};

