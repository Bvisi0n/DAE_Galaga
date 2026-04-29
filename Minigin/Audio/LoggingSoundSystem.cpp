#include <iostream>
#include <utility>

#include "Minigin/Audio/LoggingSoundSystem.h"

namespace dae::audio
{
	LoggingSoundSystem::LoggingSoundSystem( std::unique_ptr<ISoundSystem>&& soundSystem )
		: m_realSoundSystem( std::move( soundSystem ) )
	{}

	void LoggingSoundSystem::Play( sound_id id, float volume )
	{
		if ( m_realSoundSystem )
		{
			m_realSoundSystem->Play( id, volume );
		}

		// TODO dae_audio - Implement logging.
	}
}