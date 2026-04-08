#include <algorithm>
#include <cassert>
#include <chrono>
#include <cstdint>
#include <functional>
#include <iostream>
#include <sstream>
#include <string>

#if WIN32
#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <windows.h>
#endif

#if USE_STEAMWORKS && !__EMSCRIPTEN__
#pragma warning (push)
#pragma warning (disable:4996)
#include <steam_api.h>
#pragma warning (pop)
#endif

#include <SDL3/SDL_error.h>
#include <SDL3/SDL_init.h>
#include <SDL3/SDL_log.h>
#include <SDL3/SDL_stdinc.h>
#include <SDL3/SDL_timer.h>
#include <SDL3/SDL_version.h>
#include <SDL3/SDL_video.h>
#include <SDL3_ttf/SDL_ttf.h>

#include "Minigin/Core/Minigin.h"
#include "Minigin/Graphics/Renderer.h"
#include "Minigin/Input/InputManager.h"
#include "Minigin/Resources/ResourceManager.h"
#include "Minigin/Scene/SceneManager.h"

SDL_Window * g_window{};

static void LogSDLVersion( const std::string& message, int major, int minor, int patch )
{
#if WIN32
	std::stringstream ss;
	ss << message << major << "." << minor << "." << patch << "\n";
	OutputDebugString( ss.str().c_str() );
#else
	std::cout << message << major << "." << minor << "." << patch << "\n";
#endif
}

#ifdef __EMSCRIPTEN__
#include "emscripten.h"

static void LoopCallback( void* arg )
{
	static_cast<dae::core::Minigin*>( arg )->RunOneFrame();
}
#endif

static void PrintSDLVersion()
{
	LogSDLVersion( "Compiled with SDL", SDL_MAJOR_VERSION, SDL_MINOR_VERSION, SDL_MICRO_VERSION );
	int version = SDL_GetVersion();
	LogSDLVersion( "Linked with SDL ", SDL_VERSIONNUM_MAJOR( version ), SDL_VERSIONNUM_MINOR( version ), SDL_VERSIONNUM_MICRO( version ) );
	LogSDLVersion( "Compiled with SDL_ttf ", SDL_TTF_MAJOR_VERSION, SDL_TTF_MINOR_VERSION, SDL_TTF_MICRO_VERSION );
	version = TTF_Version();
	LogSDLVersion( "Linked with SDL_ttf ", SDL_VERSIONNUM_MAJOR( version ), SDL_VERSIONNUM_MINOR( version ), SDL_VERSIONNUM_MICRO( version ) );
}

namespace dae::core
{
	Minigin::Minigin( const MiniginConfig& config )
		: m_nsPerFrame( std::chrono::nanoseconds( 1'000'000'000 / std::max<uint16_t>( 1, config.targetFPS ) ) )
		, m_maxDeltaTime( 1.0f / std::max<uint16_t>( 1, config.minProcessableFPS ) )
	{
		PrintSDLVersion();

	#if USE_STEAMWORKS && !__EMSCRIPTEN__
		const bool steamInitialized = SteamAPI_Init();
		if ( steamInitialized == false )
		{
			SDL_Log( "Fatal Error: Steam must be running to play this game." );

			assert( steamInitialized == true && "SteamAPI_Init() failed" );

			return;
		}
	#endif

	#if WIN32
		const bool sdlInitialized = SDL_InitSubSystem( SDL_INIT_VIDEO );
	#else
		const bool sdlInitialized = SDL_InitSubSystem( SDL_INIT_VIDEO | SDL_INIT_GAMEPAD );
	#endif

		if ( sdlInitialized == false )
		{
			SDL_Log( "SDL_Init Error: %s", SDL_GetError() );
			assert( sdlInitialized == true && "Failed to initialize SDL SubSystems" );
			return;
		}

		g_window = SDL_CreateWindow( config.windowTitle.c_str(), config.windowWidth, config.windowHeight, SDL_WINDOW_OPENGL );

		if ( g_window == nullptr )
		{
			SDL_Log( "SDL_CreateWindow Error: %s", SDL_GetError() );
			assert( g_window != nullptr && "Failed to create SDL Window" );
			return;
		}

		graphics::Renderer::GetInstance().Init( g_window );
		resources::ResourceManager::GetInstance().Init( config.dataPath );
	}

	Minigin::~Minigin()
	{
		resources::ResourceManager::GetInstance().Destroy();
		graphics::Renderer::GetInstance().Destroy();
		SDL_DestroyWindow( g_window );
		g_window = nullptr;
		SDL_Quit();
	#if USE_STEAMWORKS && !__EMSCRIPTEN__
		SteamAPI_Shutdown();
	#endif
	}

	void Minigin::Run( const std::function<void()>& load )
	{
		if ( g_window == nullptr || !graphics::Renderer::GetInstance().IsValid() )
		{
			SDL_Log( "Engine failed to initialize successfully. Aborting Run loop." );
			return;
		}

		load();

		m_lastTime = std::chrono::steady_clock::now();
		m_quit = false;

	#ifndef __EMSCRIPTEN__
		while ( !m_quit )
		{
			RunOneFrame();
		#if USE_STEAMWORKS
			SteamAPI_RunCallbacks();
		#endif
		}
	#else
		emscripten_set_main_loop_arg( &LoopCallback, this, 0, true );
	#endif
	}

	void Minigin::RunOneFrame()
	{
		using clock = std::chrono::steady_clock; // Steady clock is guaranteed to be monotonic
		const auto frameStartTime{ clock::now() };
		const float deltaTime{ std::chrono::duration<float>( frameStartTime - m_lastTime ).count() };
		const float clampedDeltaTime = std::min( deltaTime, m_maxDeltaTime );
		m_lastTime = frameStartTime;

		m_quit = !input::InputManager::GetInstance().ProcessInput( clampedDeltaTime );

		scenes::SceneManager::GetInstance().Update( clampedDeltaTime );
		graphics::Renderer::GetInstance().Render();

		const auto frameEndTime{ clock::now() };
		const auto executionTime{ frameEndTime - frameStartTime };

		if ( executionTime < m_nsPerFrame )
		{
			const auto waitDuration = m_nsPerFrame - executionTime;
			// I opted for SDL_DelayPrecise, the last function in this file (line 664)
				// https://github.com/libsdl-org/SDL/blob/main/src/timer/SDL_timer.c
			SDL_DelayPrecise( static_cast<Uint64>( waitDuration.count() ) );
		}
	}
}
