#ifndef MINIGIN_H
#define MINIGIN_H

#include <chrono>
#include <filesystem>
#include <functional>
#include <string>

namespace dae::core
{
	class Minigin final
	{
	public:
		explicit Minigin(const std::filesystem::path& dataPath, const std::string name, const unsigned short windowWidth = 1280, const unsigned short windowHeight = 720);
		~Minigin();

		Minigin(const Minigin& other)			 = delete;
		Minigin(Minigin&& other)				 = delete;
		Minigin& operator=(const Minigin& other) = delete;
		Minigin& operator=(Minigin&& other)		 = delete;

		void Run(const std::function<void()>& load);
		void RunOneFrame();

	private:
		static constexpr float m_maxDeltaTime{ 1.0f / 10.0f }; // Set a limit to the delta time
        static constexpr std::chrono::nanoseconds m_nsPerFrame{ 1000000000 / 60 }; // 1 second in nanoseconds divided by max FPS

		std::chrono::steady_clock::time_point m_lastTime{};
		bool  m_quit{};
	};
}
#endif
