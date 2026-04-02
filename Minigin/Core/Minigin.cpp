#include <algorithm>
#include <iostream>
#include <sstream>
#include <stdexcept>

#if WIN32
	#define WIN32_LEAN_AND_MEAN
	#include <windows.h>
#endif

#if USE_STEAMWORKS && !__EMSCRIPTEN__
	#pragma warning (push)
	#pragma warning (disable:4996)
	#include <steam_api.h>
	#pragma warning (pop)
#endif

#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>

#include "Minigin/Core/Minigin.h"
#include "Minigin/Graphics/Renderer.h"
#include "Minigin/Input/InputManager.h"
#include "Minigin/Resources/ResourceManager.h"
#include "Minigin/Scene/SceneManager.h"

SDL_Window* g_window{};

void LogSDLVersion(const std::string& message, int major, int minor, int patch)
{
	#if WIN32
		std::stringstream ss;
		ss << message << major << "." << minor << "." << patch << "\n";
		OutputDebugString(ss.str().c_str());
	#else
		std::cout << message << major << "." << minor << "." << patch << "\n";
	#endif
}

#ifdef __EMSCRIPTEN__
	#include "emscripten.h"

	void LoopCallback(void* arg)
	{
		static_cast<dae::core::Minigin*>(arg)->RunOneFrame();
	}
#endif

void PrintSDLVersion()
{
	LogSDLVersion("Compiled with SDL", SDL_MAJOR_VERSION, SDL_MINOR_VERSION, SDL_MICRO_VERSION);
	int version = SDL_GetVersion();
	LogSDLVersion("Linked with SDL ", SDL_VERSIONNUM_MAJOR(version), SDL_VERSIONNUM_MINOR(version), SDL_VERSIONNUM_MICRO(version));
	LogSDLVersion("Compiled with SDL_ttf ",	SDL_TTF_MAJOR_VERSION, SDL_TTF_MINOR_VERSION,SDL_TTF_MICRO_VERSION);
	version = TTF_Version();
	LogSDLVersion("Linked with SDL_ttf ", SDL_VERSIONNUM_MAJOR(version), SDL_VERSIONNUM_MINOR(version),	SDL_VERSIONNUM_MICRO(version));
}

namespace dae::core
{
	Minigin::Minigin(const std::filesystem::path& dataPath, const std::string name, const unsigned short windowWidth, const unsigned short windowHeight)
	{
		PrintSDLVersion();

		#if USE_STEAMWORKS && !__EMSCRIPTEN__
			if (!SteamAPI_Init())
			{
				throw std::runtime_error(std::string("Fatal Error - Steam must be running to play this game (SteamAPI_Init() failed)."));
			}
		#endif

		#if WIN32
			if (!SDL_InitSubSystem(SDL_INIT_VIDEO))
		#else
			if (!SDL_InitSubSystem(SDL_INIT_VIDEO | SDL_INIT_GAMEPAD))
		#endif

		{
			SDL_Log("Renderer error: %s", SDL_GetError());
			throw std::runtime_error(std::string("SDL_Init Error: ") + SDL_GetError());
		}

		g_window = SDL_CreateWindow(name.c_str(), windowWidth, windowHeight, SDL_WINDOW_OPENGL);

		if (g_window == nullptr) 
		{
			throw std::runtime_error(std::string("SDL_CreateWindow Error: ") + SDL_GetError());
		}

		graphics::Renderer::GetInstance().Init(g_window);
		resources::ResourceManager::GetInstance().Init(dataPath);
	}

	Minigin::~Minigin()
	{
		resources::ResourceManager::GetInstance().Destroy();
		graphics::Renderer::GetInstance().Destroy();
		SDL_DestroyWindow(g_window);
		g_window = nullptr;
		SDL_Quit();
		#if USE_STEAMWORKS && !__EMSCRIPTEN__
				SteamAPI_Shutdown();
		#endif
	}

	void Minigin::Run(const std::function<void()>& load)
	{
		load();

		m_lastTime = std::chrono::steady_clock::now();
		m_quit = false;

		#ifndef __EMSCRIPTEN__
			while (!m_quit)
			{
				RunOneFrame();
				#if USE_STEAMWORKS
					SteamAPI_RunCallbacks();
				#endif
			}
		#else
			emscripten_set_main_loop_arg(&LoopCallback, this, 0, true);
		#endif
	}

	void Minigin::RunOneFrame()
	{
		using clock = std::chrono::steady_clock; // Steady clock is guaranteed to be monotonic
		const auto frame_start_time{ clock::now() };
		const float delta_time{ std::chrono::duration<float>(frame_start_time - m_lastTime).count() };
		const float clamped_delta_time = std::min(delta_time, m_maxDeltaTime);
		m_lastTime = frame_start_time;

		m_quit = !input::InputManager::GetInstance().ProcessInput(clamped_delta_time);

		scene::SceneManager::GetInstance().Update(clamped_delta_time);
		graphics::Renderer::GetInstance().Render();

		const auto frame_end_time{ clock::now() };
		const auto execution_time{ frame_end_time - frame_start_time };

		if (execution_time < m_nsPerFrame)
		{
			const auto wait_duration = m_nsPerFrame - execution_time;
			// I opted for SDL_DelayPrecise, the last function in this file (line 664): https://github.com/libsdl-org/SDL/blob/main/src/timer/SDL_timer.c
			SDL_DelayPrecise(static_cast<Uint64>(wait_duration.count()));
		}
	}
}
