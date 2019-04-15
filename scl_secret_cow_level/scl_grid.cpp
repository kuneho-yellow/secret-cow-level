#include "scl_grid.h"
#include "scl_game_object.h"

SCLGrid::SCLGrid()
	: Cells(),
	m_Width(0),
	m_Height(0),
	m_CellSize(0),
	m_CellsInRowCount(0),
	m_CellsInColCount(0)
{

}


SCLGrid::~SCLGrid()
{
}

void SCLGrid::Initialize(int Width, int Height, int CellSize)
{
	m_Width = Width;
	m_Height = Height;
	m_CellSize = CellSize;
	m_CellsInRowCount = (int)ceil((float)Width / CellSize);
	m_CellsInColCount = (int)ceil((float)Height / CellSize);

	// Create the cells
	Cells.resize(m_CellsInRowCount * m_CellsInColCount);

	// TODO: Not sure if I should reserve the cells' vectors already :I
}

void SCLGrid::AddObject(SCLGameObject* ObjectToAdd)
{
	// Returns the object's vector index in the cell
	SCLCell* Cell = GetCell(ObjectToAdd->GetPosition());
	if (Cell != NULL)
	{
		Cell->Objects.push_back(ObjectToAdd);
		ObjectToAdd->SetCellPointer(Cell);
		ObjectToAdd->SetCellVectorI(Cell->Objects.size() - 1);
	}
}

void SCLGrid::UpdateObject(SCLGameObject* ObjectToUpdate)
{
	SCLCell* NewCell = GetCell(ObjectToUpdate->GetPosition());
	SCLCell* OldCell = ObjectToUpdate->GetCellPointer();

	if (OldCell == NewCell)
	{
		// Exit if there's no cell change
		return;
	}

	// Remove from old cell. Move the last item in the vector to the vacated slot
	if (OldCell != NULL)
	{
		int OldCellVectorI = ObjectToUpdate->GetCellVectorI();
		SCLGameObject* OldCellLastItem = OldCell->Objects.back();
		if (OldCellLastItem != ObjectToUpdate)
		{
			OldCell->Objects[OldCellVectorI] = OldCellLastItem;
			OldCellLastItem->SetCellVectorI(OldCellVectorI);
		}
		OldCell->Objects.pop_back();
	}

	// Add to new cell
	if (NewCell != NULL)
	{
		NewCell->Objects.push_back(ObjectToUpdate);
		ObjectToUpdate->SetCellPointer(NewCell);
		ObjectToUpdate->SetCellVectorI(NewCell->Objects.size() - 1);
	}
	else
	{
		ObjectToUpdate->SetCellPointer(NewCell);
		ObjectToUpdate->SetCellVectorI(-1);
	}
}

void SCLGrid::RemoveObject(SCLGameObject* ObjectToRemove)
{
	// Exit if the object is not in the grid
	SCLCell* Cell = ObjectToRemove->GetCellPointer();
	int CellVectorI = ObjectToRemove->GetCellVectorI();
	if (Cell == NULL)
	{
		return;
	}

	// Remove from old cell. Move the last item in the vector to the vacated slot
	SCLGameObject* CellLastItem = Cell->Objects.back();
	if (CellLastItem != ObjectToRemove)
	{
		Cell->Objects[CellVectorI] = CellLastItem;
		CellLastItem->SetCellVectorI(CellVectorI);
	}
	Cell->Objects.pop_back();
	ObjectToRemove->SetCellPointer(NULL);
	ObjectToRemove->SetCellVectorI(-1);
}

SCLCell* SCLGrid::GetCell(SCLCellCoord CellCoord)
{
	// Return NULL for invalid coordinates
	if (CellCoord.x < 0 || CellCoord.x >= m_CellsInRowCount ||
		CellCoord.y < 0 || CellCoord.y >= m_CellsInColCount)
	{
		return NULL;
	}

	// Rows are just placed "side-by-side"
	int CellIndex = CellCoord.x + (CellCoord.y * m_CellsInRowCount);
	return &Cells[CellIndex];
}

SCLCell* SCLGrid::GetCell(glm::vec2 Position)
{
	return GetCell(GetCellCoord(Position));
}

SCLCellCoord SCLGrid::GetCellCoord(glm::vec2 Position) const
{
	// Things are adjusted so that
	// the center of the grid is at position (0.0f, 0.0f)
	SCLCellCoord CellCoord(0, 0);
	CellCoord.x = (int)((Position.x + (m_Width / 2)) / m_CellSize);
	CellCoord.y = (int)((Position.y + (m_Height / 2)) / m_CellSize);
	
	// Comment out if I want to return NULL cells
	// Clamp the coordinates
	if (CellCoord.x < 0)
	{
		CellCoord.x = 0;
	}
	if (CellCoord.x >= m_CellsInRowCount)
	{
		CellCoord.x = m_CellsInRowCount - 1;
	}
	if (CellCoord.y < 0)
	{
		CellCoord.y = 0;
	}
	if (CellCoord.y >= m_CellsInColCount)
	{
		CellCoord.y = m_CellsInColCount - 1;
	}

	return CellCoord;
}

glm::vec2 SCLGrid::GetCellBoundsX(int CellX) const
{
	int Min = (CellX * m_CellSize) - (m_Width / 2);
	glm::vec2 Bounds(Min, Min + m_CellSize);
	return Bounds;
}

glm::vec2 SCLGrid::GetCellBoundsY(int CellY) const
{
	int Min = (CellY * m_CellSize) - (m_Height / 2);
	glm::vec2 Bounds(Min, Min + m_CellSize);
	return Bounds;
}

int SCLGrid::GetRowSize() const
{
	return m_CellsInRowCount;
}

int SCLGrid::GetColumnSize() const
{
	return m_CellsInColCount;
}