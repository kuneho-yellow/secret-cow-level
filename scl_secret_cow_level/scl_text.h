#pragma once

#include "scl_game_object.h"

class SCLText : public SCLGameObject
{
public:
	SCLText();
	~SCLText();

	void Initialize(char ThisChar, glm::vec2 Position, float Size);
	void SetTextPosition(glm::vec2 Position);
	void MoveText(glm::vec2 DeltaPos);
	void SetCharacter(char ThisChar);
	char GetCharacter() const;

private:
	void AssignUVFromChar(char ThisChar);
	char m_Char;
};

