#include "scl_text.h"
#include "scl_resource_manager.h"
#include "scl_game_constants.h"

SCLText::SCLText()
	: m_Char(0)
{
}


SCLText::~SCLText()
{
}


void SCLText::Initialize(char ThisChar, glm::vec2 Position, float Size)
{
	SetSprite(SCLResourceManager::GetLoadedTexture(SCLResources::TextureType_Text), Size);
	SetPosition(Position);
	SetCharacter(ThisChar);
	SetSpriteAlpha(SCLConstants::TEXT_ALPHA);
	m_IsActive = true;
}

void SCLText::SetTextPosition(glm::vec2 Position)
{
	SetPosition(Position);
}

void SCLText::MoveText(glm::vec2 DeltaPos)
{
	Move(DeltaPos);
}

void SCLText::SetCharacter(char ThisChar)
{
	m_Char = ThisChar;
	AssignUVFromChar(ThisChar);
}

char SCLText::GetCharacter() const
{
	return m_Char;
}

void SCLText::AssignUVFromChar(char ThisChar)
{
	// I didn't get all the characters
	// TODO: Make this more proper..
	char StartChar = '.';
	int RowSize = 8;
	int ColSize = 8;

	char Offset = ThisChar - StartChar;
	float BotLeftU = (float)(Offset % RowSize) / RowSize;
	float TopRightU = BotLeftU + (1.0f / RowSize);
	float TopRightV = 1.0f - ((float)(Offset / RowSize) / ColSize);
	float BotLeftV = TopRightV - (1.0f / ColSize);

	SetUV(BotLeftU, BotLeftV, TopRightU, TopRightV);
}