#include "scl_ui_manager.h"
#include "scl_game_manager.h"
#include "scl_camera.h"
#include "scl_text.h"
#include "scl_game_constants.h"

SCLUIManager::SCLUIManager()
	: m_pGameManager(NULL),
	m_FPSCounter(),
	m_Score(),
	m_HP(),
	m_CameraPos(0.0f),
	m_Buffer(),
	m_FPSUpdateTimer(SCLConstants::FPS_DISPLAY_UPDATE_TIME)
{
}

SCLUIManager::~SCLUIManager()
{
	CleanupText(m_FPSCounter);
	CleanupText(m_Score);
	CleanupText(m_HP);
}

void SCLUIManager::Initialize(SCLGameManager* pGameManager)
{
	m_pGameManager = pGameManager;
	m_CameraPos = m_pGameManager->GetCamera()->GetPosition();
}

void SCLUIManager::Update(float DeltaTime)
{
	// Update all text
	m_FPSUpdateTimer += DeltaTime;
	if (m_FPSUpdateTimer >= SCLConstants::FPS_DISPLAY_UPDATE_TIME)
	{
		// Update the number itself only at a specified rate
		UpdateFPSCounter(true);
		m_FPSUpdateTimer = 0.0f;
	}
	else
	{
		UpdateFPSCounter(false);
	}
	UpdateScore();
	UpdateHP();

	// Draw all text
	DrawText(m_FPSCounter);
	DrawText(m_Score);
	DrawText(m_HP);

	m_CameraPos = m_pGameManager->GetCamera()->GetPosition();
}

void SCLUIManager::UpdateFPSCounter(bool IsUpdateText)
{
	if (IsUpdateText)
	{
		// FPS Counter on lower left
		sprintf_s(m_Buffer, sizeof(m_Buffer), "FPS:%.2f", m_pGameManager->GetFPS());
		glm::vec2 Position(0.0f);
		glm::vec2 CameraSize = m_pGameManager->GetCamera()->GetCameraSize();
		float Size = SCLConstants::TEXT_SIZE;
		Position.x = (Size - CameraSize.x) * 0.5f;
		Position.y = (Size - CameraSize.y) * 0.5f;
		Position += m_pGameManager->GetCamera()->GetPosition();
		UpdateText(m_Buffer, m_FPSCounter, Position, Size);
	}
	else
	{
		// Need to update the text's position in case the camera moved
		glm::vec2 DeltaPos = m_pGameManager->GetCamera()->GetPosition() - m_CameraPos;
		MoveTextPositions(m_FPSCounter, DeltaPos);
	}
}

void SCLUIManager::UpdateScore()
{
	// Score display on upper left
	sprintf_s(m_Buffer, sizeof(m_Buffer), "SCORE:%d", m_pGameManager->GetScore());
	glm::vec2 Position(0.0f);
	glm::vec2 CameraSize = m_pGameManager->GetCamera()->GetCameraSize();
	float Size = SCLConstants::TEXT_SIZE;
	Position.x = (Size - CameraSize.x) * 0.5f;
	Position.y = (CameraSize.y - Size) * 0.5f;
	Position += m_pGameManager->GetCamera()->GetPosition();
	UpdateText(m_Buffer, m_Score, Position, Size);
}

void SCLUIManager::UpdateHP()
{
	// HP display on upper left below Score
	sprintf_s(m_Buffer, sizeof(m_Buffer), "HP:%d", m_pGameManager->GetPlayerHP());
	glm::vec2 Position(0.0f);
	glm::vec2 CameraSize = m_pGameManager->GetCamera()->GetCameraSize();
	float Size = SCLConstants::TEXT_SIZE;
	Position.x = (Size - CameraSize.x) * 0.5f;
	Position.y = (CameraSize.y - (Size * 3.0f)) * 0.5f;
	Position += m_pGameManager->GetCamera()->GetPosition();
	UpdateText(m_Buffer, m_HP, Position, Size);
}

void SCLUIManager::UpdateText(char* pChars, std::vector<SCLText*>& Text, glm::vec2 Position, float Size)
{
	// Update the vector or SCLTexts to show the correct characters
	unsigned int CharI = 0;
	while (pChars[CharI] != NULL)
	{
		if (CharI < Text.size())
		{
			// Update existing character
			Text[CharI]->Initialize(pChars[CharI], Position, Size);
		}
		else
		{
			// Push back new character
			SCLText* pNewChar = new SCLText();
			pNewChar->Initialize(pChars[CharI], Position, Size);
			Text.push_back(pNewChar);
		}
		Position.x += Size * 0.5f;
		CharI++;
	}
	while (CharI < Text.size())
	{
		// Make unneeded characters invisible
		Text[CharI++]->SetCharacter(' ');
	}
}

void SCLUIManager::MoveTextPositions(std::vector<SCLText*>& Text, glm::vec2 DeltaPos)
{
	for (unsigned int CharI = 0; CharI < Text.size(); ++CharI)
	{
		Text[CharI]->MoveText(DeltaPos);
	}
}

void SCLUIManager::DrawText(std::vector<SCLText*>& Text)
{
	// Add each SCLText sprite to the drawing board
	for (unsigned int CharI = 0; CharI < Text.size(); ++CharI)
	{
		// Except those showing a space..
		if (Text[CharI]->GetCharacter() != ' ')
		{
			m_pGameManager->AddSprite(Text[CharI]->GetSprite());
		}
	}
}

void SCLUIManager::CleanupText(std::vector<SCLText*>& Text)
{
	// Free up allocations
	for (unsigned int CharI = 0; CharI < Text.size(); ++CharI)
	{
		if (Text[CharI] != NULL)
		{
			delete Text[CharI];
		}
	}
	Text.clear();
}