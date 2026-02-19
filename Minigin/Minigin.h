#ifndef MINIGIN_H
#define MINIGIN_H

#include <chrono>
#include <filesystem>
#include <functional>
#include <string>

namespace dae
{
	class Minigin final
	{
	public:
		explicit Minigin(const std::filesystem::path& dataPath);
		~Minigin();

		Minigin(const Minigin& other)			 = delete;
		Minigin(Minigin&& other)				 = delete;
		Minigin& operator=(const Minigin& other) = delete;
		Minigin& operator=(Minigin&& other)		 = delete;

		void Run(const std::function<void()>& load);
		void RunOneFrame();

	private:
		static constexpr int m_desiredPhysicsFPS{ 120 };
		static constexpr int m_desiredRenderFPS{ 60 };
		static constexpr float m_fixedTimeStep{ 1.0f / m_desiredPhysicsFPS };
		static constexpr std::chrono::nanoseconds m_nsPerFrame{ 1000000000 / m_desiredRenderFPS };

		std::chrono::steady_clock::time_point m_lastTime{};
		bool  m_quit{};
	};
}
#endif
