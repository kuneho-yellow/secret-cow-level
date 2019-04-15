#pragma once

#include <SDL/SDL.h>
#include <glm/glm.hpp>

class SCLGameManager;

class SCLInputManager
{
public:
	SCLInputManager();
	~SCLInputManager();

	void UpdateInput();
	void SetGameManager(SCLGameManager* pGameManager);
	glm::vec2 GetMouseWorldPos() const;

private:
	void ProcessKeyDown(SDL_Keycode KeyCode);
	void ProcessKeyUp(SDL_Keycode KeyCode);
	void ProcessMouseDown(Uint8 MouseButton);
	void ProcessMouseUp(Uint8 MouseButton);
	void ProcessMouseMove(Sint32 ScreenX, Sint32 ScreenY);

	SCLGameManager* m_pGameManager;
	glm::vec2 m_mouseWorldPos;
};

