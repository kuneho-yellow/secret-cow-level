#pragma once

#include "scl_game_manager.h"
#include "scl_visual_manager.h"

enum SCLGameState
{
	NONE,
	INITIALIZED,
	RUNNING,
	END,
	QUIT
};


class SCLMain
{
public:
	SCLMain();
	~SCLMain();

	void Run();
	void NotifyEnd();
	void NotifyQuit();

	float GetFPS();

private:
	void GameLoop();
	void MainGame();
	void EndGame();
	float ComputeFPS(Uint32 DeltaTicks);

	// Components
	SCLGameManager m_GameManager;
	SCLVisualManager m_VisualManager;

	// Timing
	float m_FPSAve;
	Uint32 m_TicksPhysics;
	float m_DeltaTimePhysics;
	Uint32 m_TicksMax;
	Uint32 m_PrevTicks;
	Uint32 m_LagTicks;

	// State
	SCLGameState m_GameState;
};

