#ifndef DAE_MINIGIN_H
#define DAE_MINIGIN_H

#include <chrono>
#include <cstdint>
#include <filesystem>
#include <string>

struct SDL_Window;

namespace dae::core
{
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
		SDL_Window* m_window{};
		std::chrono::steady_clock::time_point m_lastTime{};
		const std::chrono::nanoseconds m_nsPerFrame;
		const float m_maxDeltaTime;
		bool m_quit{};
	};
}
#endif
