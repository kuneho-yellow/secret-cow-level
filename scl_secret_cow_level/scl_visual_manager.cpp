#include "scl_visual_manager.h"
#include "scl_game_constants.h"

#include <glm/glm.hpp>

SCLVisualManager::SCLVisualManager()
	: m_pSDLWindow(NULL),
	m_ShaderProgram(),
	m_SpriteDrawer(),
	m_Camera()
{
}

SCLVisualManager::~SCLVisualManager()
{
}

bool SCLVisualManager::Initialize(int ScreenWidth, int ScreenHeight,
								  const std::string& WindowName)
{
	// Create an OpenGL window
	m_pSDLWindow = SDL_CreateWindow(WindowName.c_str(),
		SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
		ScreenWidth, ScreenHeight, SDL_WINDOW_OPENGL);
	if (m_pSDLWindow == NULL)
	{
		printf("ERROR: SDL Window could not be created!\n");
		return false;
	}

	// Initialize OpenGL context
	SDL_GLContext pGLContext = SDL_GL_CreateContext(m_pSDLWindow);
	if (pGLContext == NULL)
	{
		printf("ERROR: OpenGL context could not be created!\n");
		printf(SDL_GetError());
		return false;
	}

	// Initialize glew
	if (glewInit() != GLEW_OK)
	{
		printf("ERROR: Could not initialize glew!\n");
		return false;
	}

	// Set the background color
	glClearColor(0.5f, 0.5f, 1.0f, 1.0);

	// Enable alpha blending
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// Set VSYNC ON if you don't want "screen tearing"
	// SDL_GL_SetSwapInterval(1);

	// Initialize the shader program
	if (!m_ShaderProgram.Initialize())
	{
		printf("ERROR: Could not initialize shaders!\n");
		return false;
	}
	// Initialize the shader program
	if (!m_SpriteDrawer.Initialize())
	{
		printf("ERROR: Could not initialize sprite drawer!\n");
		return false;
	}

	// Set window width == 50 world units
	m_Camera.Initialize(ScreenWidth, ScreenHeight, SCLConstants::SCREEN_WIDTH_WORLD_SIZE);
	
	return true;
}

void SCLVisualManager::Render(float LagFactor, const std::vector<const SCLSprite*>* pSpritesToDraw)
{
	m_Camera.Update();

	// Clear the depth and color buffer
	glClearDepth(1.0);
	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

	m_ShaderProgram.Start();

	// Camera projection matrix
	glm::mat4 CameraMatrix = m_Camera.GetVPMatix();
	m_ShaderProgram.SetCameraMatrix(&(CameraMatrix[0][0]));

	// Draw stuff here
	m_SpriteDrawer.DrawSprites(pSpritesToDraw);

	m_ShaderProgram.End();

	// Swap the double buffers
	SDL_GL_SwapWindow(m_pSDLWindow);
}

SCLCamera* SCLVisualManager::GetCamera()
{
	return &m_Camera;
}
