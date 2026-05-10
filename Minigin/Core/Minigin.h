#ifndef DAE_MINIGIN_H
#define DAE_MINIGIN_H

#include <chrono>
#include <cstdint>
#include <filesystem>
#include <memory>
#include <string>

#include <SDL3/SDL_video.h>

// TODO dae_cmake - Properly encapsulate external dependencies so that they don't show up as projects in solution explorer.

namespace dae::core
{
	struct SDLWindowDestroyer
	{
		void operator()( SDL_Window* window ) const noexcept
		{
			if ( window != nullptr )
			{
				SDL_DestroyWindow( window );
			}
		}
	};

	using UniqueWindow = std::unique_ptr<SDL_Window, SDLWindowDestroyer>;

	struct MiniginDescriptor
	{
		std::filesystem::path dataPath{ "./Data/" };
		std::string windowTitle{ "Minigin Engine" };
		uint16_t windowWidth{ 1280 };
		uint16_t windowHeight{ 720 };
		uint16_t targetFPS{ 60 };
		uint16_t minProcessableFPS{ 10 };
	};

	class Minigin final
	{
	public:
		explicit Minigin( const MiniginDescriptor& descriptor );
		~Minigin();

		Minigin( const Minigin& other ) = delete;
		Minigin( Minigin&& other ) = delete;
		Minigin& operator=( const Minigin& other ) = delete;
		Minigin& operator=( Minigin&& other ) = delete;

		void Run();
		void RunOneFrame();

	private:
		UniqueWindow m_window;
		std::chrono::steady_clock::time_point m_lastTime;
		const std::chrono::nanoseconds m_nsPerFrame;
		const float m_maxDeltaTime;
		bool m_quit{};
	};
}
#endif
