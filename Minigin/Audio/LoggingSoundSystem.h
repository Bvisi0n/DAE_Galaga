#ifndef DAE_LOGGINGSOUNDSYSTEM_H
#define DAE_LOGGINGSOUNDSYSTEM_H

#include <filesystem>
#include <memory>

#include <Minigin/Audio/ISoundSystem.h>

namespace dae::audio
{
	class LoggingSoundSystem final : public ISoundSystem
	{
	public:
		explicit LoggingSoundSystem( std::unique_ptr<ISoundSystem>&& soundSystem );
		~LoggingSoundSystem() override = default;

		LoggingSoundSystem( const LoggingSoundSystem& ) = delete;
		LoggingSoundSystem( LoggingSoundSystem&& ) = default;
		LoggingSoundSystem& operator=( const LoggingSoundSystem& ) = delete;
		LoggingSoundSystem& operator=( LoggingSoundSystem&& ) = default;

		void Play( const PlayMessage& message ) override;
		void RegisterSound( SoundID soundID, const std::filesystem::path& filepath ) override;

	private:
		std::unique_ptr<ISoundSystem> m_realSoundSystem;
	};
}
#endif
