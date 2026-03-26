#include "SDLRenderer.h"
#include <SDL.h>
#include <iostream>

extern "C"
{
#include <SDL2_gfxPrimitives.h>
}

#include "imgui.h"
#include "imgui_impl_sdl2.h"
#include "imgui_impl_sdlrenderer2.h"

#include "BruteForceSimulator.h"
#include "GridSimluator.h"

SDLRenderer::SDLRenderer()
	: m_window(nullptr)
	, m_renderer(nullptr)
	, m_isRun(true)
	, m_simData{}
	, m_isSimulate(false)
	, m_fps(0)
	, m_simulCount(1)
	, m_simulTimer(0)
	, m_selected(0)
{
	m_simData.m_poolSize[0] = 800;
	m_simData.m_poolSize[1] = 600;
	m_simData.m_count = 100;
	m_simData.m_radius = 1;
	m_simData.m_simSpeed = 1;

	m_simulators[0] = new BruteForceSimulator();
	m_simulatorsName[0] = "Brute Force";
	m_simulators[1] = new GridSimluator();
	m_simulatorsName[1] = "Grid";
}

SDLRenderer::~SDLRenderer()
{
	SDL_DestroyRenderer(m_renderer);
	SDL_DestroyWindow(m_window);
	SDL_Quit();
}

void SDLRenderer::InitWindow()
{
	if (0 > SDL_Init(SDL_INIT_VIDEO))
	{
		std::cout << "cannot create SDL window\n";
		return;
	}

	m_window = SDL_CreateWindow
	(
		"Collision Algoprithm Test"
		, SDL_WINDOWPOS_CENTERED
		, SDL_WINDOWPOS_CENTERED
		, m_simData.m_poolSize[0]
		, m_simData.m_poolSize[1]
		, SDL_WINDOW_SHOWN
	);

	if (nullptr == m_window)
	{
		std::cout << "create SDL window fail\n";
		return;
	}

	m_renderer = SDL_CreateRenderer
	(
		m_window
		, -1
		, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC
	);

	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;

	ImGui_ImplSDL2_InitForSDLRenderer(m_window, m_renderer);
	ImGui_ImplSDLRenderer2_Init(m_renderer);
}

void SDLRenderer::SDLLoop()
{
	SDL_Event event;

	while (true == m_isRun)
	{
		while (SDL_PollEvent(&event))
		{
			ImGui_ImplSDL2_ProcessEvent(&event);
			if (event.type == SDL_QUIT)
			{
				m_isRun = false;
			}
		}

		SDL_SetRenderDrawColor(m_renderer, 30, 30, 30, 255);
		SDL_RenderClear(m_renderer);

		// ·»´õ ÀÌº¥Æ®
		if (true == m_isSimulate)
		{
			auto beginFrame = std::chrono::high_resolution_clock::now();
			m_simulators[m_selected]->Simulate(m_simData.m_simSpeed);
			auto endFrame = std::chrono::high_resolution_clock::now();

			std::chrono::duration<double, std::milli> simDuration = endFrame - beginFrame;
			m_simulTimer += simDuration.count();
			m_fps = m_simulTimer / m_simulCount;

			m_simulCount++;
			for (const auto& collider : m_simulators[m_selected]->GetColliderData())
			{
				filledCircleRGBA(
					m_renderer
					, static_cast<int>(collider.m_x)
					, static_cast<int>(collider.m_y)
					, static_cast<int>(m_simData.m_radius)
					, 100, 200, 255, 255
				);
			}
		}

		ImGui_ImplSDLRenderer2_NewFrame();
		ImGui_ImplSDL2_NewFrame();
		ImGui::NewFrame();

		ImGui::Begin("Simulate Data Setting");

		ImGui::InputInt("Collider Count", &m_simData.m_count);
		ImGui::InputInt("Collider Radius", &m_simData.m_radius);
		ImGui::InputInt2("Pool Size", m_simData.m_poolSize);
		ImGui::SameLine();
		if (ImGui::Button("Reset Window Size"))
		{
			SDL_SetWindowSize(m_window, m_simData.m_poolSize[0], m_simData.m_poolSize[1]);
		}
		ImGui::InputInt("Simulate Speed", &m_simData.m_simSpeed);

		ImGui::Combo("Simulator Select", &m_selected, m_simulatorsName, IM_ARRAYSIZE(m_simulatorsName));



		if (true == m_isSimulate)
		{
			if (true == ImGui::Button("Simulate Stop"))
			{
				m_isSimulate = false;
			}
		}
		else
		{
			if (true == ImGui::Button("Simulate Start"))
			{
				m_simulators[m_selected]->SetSimulData(m_simData);
				m_isSimulate = true;
				m_fps = 0;
				m_simulCount = 1;
				m_simulTimer = 0;
			}
		}

		ImGui::SameLine();
		ImGui::Text("fps : %lf", m_fps);

		ImGui::End();

		ImGui::Render();
		ImGui_ImplSDLRenderer2_RenderDrawData(ImGui::GetDrawData(), m_renderer);

		SDL_RenderPresent(m_renderer);
	}
}

