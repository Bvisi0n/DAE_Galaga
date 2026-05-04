#include <filesystem>
#include <memory>
#include <utility>

#include <Minigin/Audio/ISoundSystem.h>
#include <Minigin/Audio/LoggingSoundSystem.h>

// TODO dae_audio - Implement imgui logging.

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
	}

	void LoggingSoundSystem::RegisterSound( SoundID soundID, const std::filesystem::path& filepath )
	{
		if ( m_realSoundSystem != nullptr )
		{
			m_realSoundSystem->RegisterSound( soundID, filepath );
		}
	}
}
