#ifndef DAE_LOGGINGSOUNDSYSTEM_H
#define DAE_LOGGINGSOUNDSYSTEM_H

#include <memory>

#include "Minigin/Audio/ISoundSystem.h"

namespace dae::audio
{
	class LoggingSoundSystem final : public ISoundSystem
	{
	public:
		explicit LoggingSoundSystem( std::unique_ptr<ISoundSystem>&& soundSystem );
		~LoggingSoundSystem() = default;

		LoggingSoundSystem( const LoggingSoundSystem& ) = delete;
		LoggingSoundSystem( LoggingSoundSystem&& ) = default;
		LoggingSoundSystem& operator=( const LoggingSoundSystem& ) = delete;
		LoggingSoundSystem& operator=( LoggingSoundSystem&& ) = default;

		void Play( const sound_id id, const float volume ) override;

	private:
		std::unique_ptr<ISoundSystem> m_realSoundSystem;
	};
}
#endif
