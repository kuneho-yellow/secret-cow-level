#include "scl_main.h"
#include "scl_game_constants.h"
#include "scl_resource_manager.h"
#include "scl_random.h"

SCLMain::SCLMain()
	: m_GameManager(),
	m_VisualManager(),
	m_FPSAve(0.0f),
	m_TicksPhysics((Uint32)(1000.0f / SCLConstants::FPS_FIXED)),
	m_DeltaTimePhysics(m_TicksPhysics / 1000.0f),
	m_TicksMax((Uint32)(1000.0f / SCLConstants::FPS_MAX)),
	m_PrevTicks(0),
	m_LagTicks(0),
	m_GameState(NONE)
{
	
}

SCLMain::~SCLMain()
{
}

void SCLMain::Run()
{
	// Initialization
	SCLRandom::Initialize();
	if (!m_GameManager.Initialize(this))
	{
		printf("ERROR: Failed to initialize the game\n");
	}
	if (!m_VisualManager.Initialize(SCLConstants::SCREEN_WIDTH, SCLConstants::SCREEN_HEIGHT, SCLConstants::NAME))
	{
		printf("ERROR: Failed to initialize graphics\n");
	}
	if (!SCLResourceManager::Initialize())
	{
		printf("ERROR: Failed to initialize resources\n");
	}
	m_GameState = INITIALIZED;

	m_GameManager.StartGame(m_VisualManager.GetCamera());
	m_GameState = RUNNING;

	// Run the game loop
	m_PrevTicks = SDL_GetTicks();
	m_LagTicks = 0;
	while (m_GameState != QUIT)
	{
		GameLoop();
	}
}

void SCLMain::NotifyEnd()
{
	m_GameState = END;
}

void SCLMain::NotifyQuit()
{
	m_GameState = QUIT;
}

float SCLMain::GetFPS()
{
	return m_FPSAve;
}

void SCLMain::GameLoop()
{
	switch (m_GameState)
	{
	case RUNNING:
		MainGame();
		break;

	case END:
		EndGame();
		break;
	default:
		break;
	}
}

void SCLMain::MainGame()
{
	// Compute the milliseconds elapsed
	Uint32 CurrTicks = SDL_GetTicks();
	Uint32 DeltaTicks = CurrTicks - m_PrevTicks;
	m_LagTicks += DeltaTicks;

	// Logic/Input update
	m_GameManager.Update(DeltaTicks / 1000.0f);
	// Physics update
	int PhysicsUpdateCounter = 0;
	while (m_LagTicks >= m_TicksPhysics && PhysicsUpdateCounter < 5)
	{
		m_GameManager.FixedUpdate(m_DeltaTimePhysics);
		m_LagTicks -= m_TicksPhysics;
		PhysicsUpdateCounter++;
	}

	// Screen update
	m_VisualManager.Render((float)m_LagTicks / m_TicksPhysics, m_GameManager.GetSprites());

	// Do nothing if there's still lots of time? :<
	// Milliseconds elapsed after all processing
	if (SCLConstants::IS_FPS_CAPPED)
	{
		Uint32 ProcessTicks = SDL_GetTicks() - CurrTicks;
		if (m_TicksMax > (DeltaTicks + ProcessTicks))
		{
			int SleepTicks = m_TicksMax - DeltaTicks - ProcessTicks;
			SDL_Delay(SleepTicks);
		}
	}

	// Compute the current average FPS
	m_FPSAve = ComputeFPS(DeltaTicks);
	m_PrevTicks = CurrTicks;
	//printf("FPS: %f\n", m_FPSAve);
}

void SCLMain::EndGame()
{
	SDL_Event ThisEvent;
	while (SDL_PollEvent(&ThisEvent))
	{
		switch (ThisEvent.type)
		{
		case SDL_QUIT:
			m_GameState = QUIT;
			break;
		}
	}
}

float SCLMain::ComputeFPS(Uint32 DeltaTicks)
{
	// Average the FPS over a certain number of frames
	static Uint32 FrameTicks[SCLConstants::FPS_SAMPLES_COUNT];
	static int CurrFrame = 0;
	static bool AreSamplesFilled = false;

	FrameTicks[CurrFrame % SCLConstants::FPS_SAMPLES_COUNT] = DeltaTicks;

	CurrFrame += 1;
	if (CurrFrame > SCLConstants::FPS_SAMPLES_COUNT)
	{
		AreSamplesFilled = true;
		CurrFrame = 0;
	}

	int SamplesCount = (AreSamplesFilled) ? CurrFrame : SCLConstants::FPS_SAMPLES_COUNT;
	float FrameTicksAve = 0.0f;
	for (int i = 0; i < SamplesCount; ++i)
	{
		FrameTicksAve += FrameTicks[i];
	}
	FrameTicksAve /= SamplesCount;
	return (FrameTicksAve > 0) ? (1000.0f / FrameTicksAve) : SCLConstants::FPS_MAX;
}