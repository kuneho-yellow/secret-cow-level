#pragma once

#include <vector>
#include <glm/glm.hpp>

class SCLGameManager;
class SCLText;

class SCLUIManager
{
public:
	SCLUIManager();
	~SCLUIManager();

	void Initialize(SCLGameManager* pGameManager);
	void Update(float DeltaTime);

private:
	void UpdateFPSCounter(bool IsUpdateText);
	void UpdateScore();
	void UpdateHP();
	void UpdateText(char* pChars, std::vector<SCLText*>& Text, glm::vec2 Position, float Size);
	void MoveTextPositions(std::vector<SCLText*>& Text, glm::vec2 DeltaPos);
	void UpdateTextPositions(std::vector<SCLText*>& Text, glm::vec2 NewPos);
	void DrawText(std::vector<SCLText*>& Text);
	void CleanupText(std::vector<SCLText*>& Text);

	// For getting info to display
	SCLGameManager* m_pGameManager;
	// Each text is an array of SCLTexts
	std::vector<SCLText*> m_FPSCounter;
	std::vector<SCLText*> m_Score;
	std::vector<SCLText*> m_HP;
	glm::vec2 m_CameraPos;
	char m_Buffer[128];
	float m_FPSUpdateTimer;
};

