#include <filesystem>
#include <memory>
#include <utility>

#include <Minigin/Audio/ISoundSystem.h>
#include <Minigin/Audio/LoggingSoundSystem.h>

namespace dae::audio
{
	LoggingSoundSystem::LoggingSoundSystem( std::unique_ptr<ISoundSystem>&& soundSystem )
		: m_realSoundSystem( std::move( soundSystem ) )
	{}

	void LoggingSoundSystem::Play( const PlayMessage& message )
	{
		if ( m_realSoundSystem != nullptr )
		{
			m_realSoundSystem->Play( message );
		}

		// TODO dae_audio - Implement imgui logging.
	}

	void LoggingSoundSystem::RegisterSound( SoundID soundID, const std::filesystem::path& filepath )
	{
		if ( m_realSoundSystem != nullptr )
		{
			m_realSoundSystem->RegisterSound( soundID, filepath );
		}

		// TODO dae_audio - Implement imgui logging.
	}
}
