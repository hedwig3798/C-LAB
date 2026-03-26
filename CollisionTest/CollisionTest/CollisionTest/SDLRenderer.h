#pragma once
#include <chrono>
#include <string>
#include "ColliderSimulator.h"

struct SDL_Window;
struct SDL_Renderer;

class SDLRenderer
{
public:
	SDLRenderer();
	virtual ~SDLRenderer();

private:
	SDL_Window* m_window;
	SDL_Renderer* m_renderer;

	Simulatedata m_simData;

	ColliderSimulator* m_simulators[2];
	const char* m_simulatorsName[2];
	int m_selected;

	bool m_isRun;
	bool m_isSimulate;

	double m_fps;
	int m_simulCount;
	double m_simulTimer;

public:
	void InitWindow();
	void SDLLoop();
};