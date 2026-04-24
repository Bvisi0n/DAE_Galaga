#ifndef DAE_MINIGIN_H
#define DAE_MINIGIN_H

#include <chrono>
#include <cstdint>
#include <filesystem>
#include <string>

namespace dae::core
{
	struct MiniginConfig
	{
		// C.1: Organize related data into structures (structs or classes)
		// I.23: Keep the number of function arguments low.
		// I.24: Avoid adjacent parameters that can be invoked by the same arguments in either order with different meaning.

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
		explicit Minigin( const MiniginConfig& config );
		~Minigin();

		Minigin( const Minigin& other ) = delete;
		Minigin( Minigin&& other ) = delete;
		Minigin& operator=( const Minigin& other ) = delete;
		Minigin& operator=( Minigin&& other ) = delete;

		void Run();
		void RunOneFrame();

	private:
		std::chrono::steady_clock::time_point m_lastTime{};
		const std::chrono::nanoseconds m_nsPerFrame;
		const float m_maxDeltaTime;
		bool m_quit{};
	};
}
#endif
