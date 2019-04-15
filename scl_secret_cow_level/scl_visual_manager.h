#pragma once

#include <SDL/SDL.h>
#include <GL/glew.h>
#include <string>

#include "scl_shader_program.h"
#include "scl_sprite_drawer.h"
#include "scl_camera.h"
#include "scl_resource_manager.h"

class SCLVisualManager
{
public:
	SCLVisualManager();
	~SCLVisualManager();

	bool Initialize(int ScreenWidth, int ScreenHeight,
					const std::string& WindowName);
	void Render(float LagFactor, const std::vector<const SCLSprite*>* pSpritesToDraw);

	SCLCamera* GetCamera();

private:
	// Window
	SDL_Window* m_pSDLWindow;
	SCLShaderProgram m_ShaderProgram;
	SCLSpriteDrawer m_SpriteDrawer;
	SCLCamera m_Camera;
};