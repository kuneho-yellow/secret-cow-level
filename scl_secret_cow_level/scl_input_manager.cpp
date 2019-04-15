#include "scl_input_manager.h"
#include "scl_game_manager.h"
#include "scl_camera.h"

SCLInputManager::SCLInputManager()
	: m_pGameManager(NULL),
	m_mouseWorldPos(0.0f)
{
}


SCLInputManager::~SCLInputManager()
{
}

void SCLInputManager::UpdateInput()
{
	SDL_Event ThisEvent;
	while (SDL_PollEvent(&ThisEvent))
	{
		switch (ThisEvent.type)
		{
		case SDL_QUIT:
			m_pGameManager->NotifyQuitPressed();
			break;

		case SDL_KEYDOWN:
			ProcessKeyDown(ThisEvent.key.keysym.sym);
			break;

		case SDL_KEYUP:
			ProcessKeyUp(ThisEvent.key.keysym.sym);
			break;

		case SDL_MOUSEBUTTONDOWN:
			ProcessMouseDown(ThisEvent.button.button);
			break;

		case SDL_MOUSEBUTTONUP:
			ProcessMouseUp(ThisEvent.button.button);
			break;

		case SDL_MOUSEMOTION:
			ProcessMouseMove(ThisEvent.motion.x, ThisEvent.motion.y);
			break;
		}
	}
}

void SCLInputManager::SetGameManager(SCLGameManager* pGameManager)
{
	m_pGameManager = pGameManager;
}

glm::vec2 SCLInputManager::GetMouseWorldPos() const
{
	return m_mouseWorldPos;
}

void SCLInputManager::ProcessKeyDown(SDL_Keycode KeyCode)
{
	if (m_pGameManager == NULL)
	{
		return;
	}
	switch (KeyCode)
	{
	case SDLK_w:
		m_pGameManager->MovePlayer(MOVE_DIR_UP);
		break;
	case SDLK_s:
		m_pGameManager->MovePlayer(MOVE_DIR_DOWN);
		break;
	case SDLK_a:
		m_pGameManager->MovePlayer(MOVE_DIR_LEFT);
		break;
	case SDLK_d:
		m_pGameManager->MovePlayer(MOVE_DIR_RIGHT);
		break;
	case SDLK_RIGHT:
		m_pGameManager->RotatePlayerFiringDir(true);
		break;
	case SDLK_LEFT:
		m_pGameManager->RotatePlayerFiringDir(false);
		break;
	}
}

void SCLInputManager::ProcessKeyUp(SDL_Keycode KeyCode)
{
	if (m_pGameManager == NULL)
	{
		return;
	}
	switch (KeyCode)
	{
	case SDLK_w:
		m_pGameManager->StopPlayer(MOVE_DIR_UP);
		break;
	case SDLK_s:
		m_pGameManager->StopPlayer(MOVE_DIR_DOWN);
		break;
	case SDLK_a:
		m_pGameManager->StopPlayer(MOVE_DIR_LEFT);
		break;
	case SDLK_d:
		m_pGameManager->StopPlayer(MOVE_DIR_RIGHT);
		break;
	case SDLK_RIGHT:
		m_pGameManager->StopRotatePlayerFiringDir();
		break;
	case SDLK_LEFT:
		m_pGameManager->StopRotatePlayerFiringDir();
		break;
	}
}

void SCLInputManager::ProcessMouseDown(Uint8 MouseButton)
{
	if (m_pGameManager == NULL)
	{
		return;
	}
	if (MouseButton == 1) // Left mouse button
	{
		m_pGameManager->SetPlayerAttackState(true);
	}
}

void SCLInputManager::ProcessMouseUp(Uint8 MouseButton)
{
	if (m_pGameManager == NULL)
	{
		return;
	}
	if (MouseButton == 1) // Left mouse button
	{
		m_pGameManager->SetPlayerAttackState(false);
	}
}

void SCLInputManager::ProcessMouseMove(Sint32 ScreenX, Sint32 ScreenY)
{
	if (m_pGameManager == NULL)
	{
		return;
	}
	if (m_pGameManager->GetCamera() != NULL)
	{
		glm::vec2 ScreenPos(ScreenX, ScreenY);
		glm::vec2 WorldPos = m_pGameManager->GetCamera()->ConvertScreenToWorld(ScreenPos);
		m_pGameManager->SetPlayerAttackPos(WorldPos);
	}
}