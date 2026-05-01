#include <cassert>
#include <condition_variable>
#include <filesystem>
#include <limits>
#include <memory>
#include <mutex>
#include <queue>
#include <stop_token>
#include <string>
#include <thread>
#include <vector>

#include <SDL3/SDL_audio.h>
#include <SDL3_mixer/SDL_mixer.h>

#include <Minigin/Audio/ISoundSystem.h>
#include <Minigin/Audio/SDLSoundSystem.h>

// https://wiki.libsdl.org/SDL3_mixer/CategorySDLMixer
// https://lazyfoo.net/tutorials/SDL3/15-sound-effects-and-music/index.php

namespace dae::audio
{
	struct MixerDeleter
	{
		void operator()( MIX_Mixer* mixer ) const
		{
			if ( mixer != nullptr )
			{
				MIX_DestroyMixer( mixer );
			}
		}
	};

	struct AudioDeleter
	{
		void operator()( MIX_Audio* audio ) const
		{
			if ( audio != nullptr )
			{
				MIX_DestroyAudio( audio );
			}
		}
	};

	using UniqueMixer = std::unique_ptr<MIX_Mixer, MixerDeleter>;
	using UniqueAudio = std::unique_ptr<MIX_Audio, AudioDeleter>;

	struct AudioClip
	{
		std::filesystem::path filepath;
		UniqueAudio audio{ nullptr };

		[[nodiscard]] bool IsLoaded() const
		{
			return audio != nullptr;
		}
	};

	class SDLSoundSystem::SDLSoundSystemImpl
	{
	public:
		SDLSoundSystemImpl()
		{
			if ( !MIX_Init() )
			{
				assert( false && "Failed to initialize SDL3_mixer" );
				return;
			}

			m_mixer.reset( MIX_CreateMixerDevice( SDL_AUDIO_DEVICE_DEFAULT_PLAYBACK, nullptr ) );
			if ( m_mixer == nullptr )
			{
				assert( false && "Failed to create SDL3_mixer device" );
				return;
			}

			m_tracks.reserve( 16 );
			for ( int i = 0; i < 16; ++i )
			{
				m_tracks.push_back( MIX_CreateTrack( m_mixer.get() ) );
			}

			m_workerThread =
				std::jthread
				(
					[ this ] ( const std::stop_token& stopToken )
					{
						ProcessQueue( stopToken );
					}
				);
		}

		~SDLSoundSystemImpl()
		{
			m_workerThread.request_stop();
			if ( m_workerThread.joinable() )
			{
				m_workerThread.join();
			}

			if ( m_mixer != nullptr )
			{
				for ( MIX_Track* track : m_tracks )
				{
					if ( track != nullptr )
					{
						MIX_StopTrack( track, 0 );
						MIX_SetTrackAudio( track, nullptr );
						MIX_DestroyTrack( track );
					}
				}
			}

			m_tracks.clear();
			m_audioCache.clear();
			m_mixer.reset();

			MIX_Quit();
		}

		SDLSoundSystemImpl( const SDLSoundSystemImpl& ) = delete;
		SDLSoundSystemImpl( SDLSoundSystemImpl&& ) = delete;
		SDLSoundSystemImpl& operator=( const SDLSoundSystemImpl& ) = delete;
		SDLSoundSystemImpl& operator=( SDLSoundSystemImpl&& ) = delete;

		void Play( const PlayMessage& message )
		{
			std::lock_guard<std::mutex> lock( m_queueMutex );
			m_queue.push( message );
			m_queueCondition.notify_one();
		}

		void RegisterSound( SoundID soundID, const std::filesystem::path& filepath )
		{
			if ( soundID >= m_audioCache.size() )
			{
				assert( soundID < std::numeric_limits<SoundID>::max() && "Sound ID exceeds maximum value" );
				m_audioCache.resize( static_cast<size_t>( soundID ) + 1 );
			}
			m_audioCache[ soundID ].filepath = filepath;
		}

	private:
		UniqueMixer m_mixer;
		std::vector<MIX_Track*> m_tracks;
		std::vector<AudioClip> m_audioCache;

		std::queue<PlayMessage> m_queue;
		std::mutex m_queueMutex;
		std::condition_variable_any m_queueCondition;

		std::jthread m_workerThread;

		void ProcessQueue( const std::stop_token& stopToken )
		{
			while ( !stopToken.stop_requested() )
			{
				PlayMessage currentMessage{};

				{
					std::unique_lock<std::mutex> lock( m_queueMutex );
					m_queueCondition.wait( lock, stopToken, [ this ] () {
						return !m_queue.empty();
					} );

					if ( stopToken.stop_requested() && m_queue.empty() )
					{
						break;
					}

					if ( m_queue.empty() )
					{
						continue;
					}

					currentMessage = m_queue.front();
					m_queue.pop();
				}

				ProcessPlayMessage( currentMessage );
			}
		}

		void ProcessPlayMessage( const PlayMessage& message )
		{
			if ( message.soundID >= m_audioCache.size() )
			{
				return;
			}

			AudioClip& clip = m_audioCache[ message.soundID ];

			if ( !clip.IsLoaded() )
			{
				clip.audio.reset( MIX_LoadAudio( m_mixer.get(), clip.filepath.string().c_str(), true ) );

				if ( !clip.IsLoaded() )
				{
					return;
				}
			}

			MIX_Track* availableTrack = nullptr;
			for ( MIX_Track* track : m_tracks )
			{
				if ( track != nullptr && !MIX_TrackPlaying( track ) )
				{
					availableTrack = track;
					break;
				}
			}

			if ( availableTrack != nullptr )
			{
				MIX_SetTrackAudio( availableTrack, clip.audio.get() );
				MIX_SetTrackGain( availableTrack, message.volume );
				MIX_PlayTrack( availableTrack, 0 );
			}
		}
	};

	SDLSoundSystem::SDLSoundSystem() : m_pimpl( std::make_unique<SDLSoundSystemImpl>() )
	{}

	SDLSoundSystem::~SDLSoundSystem() = default;

	void SDLSoundSystem::Play( const PlayMessage& message )
	{
		m_pimpl->Play( message );
	}

	void SDLSoundSystem::RegisterSound( SoundID soundID, const std::filesystem::path& filepath )
	{
		m_pimpl->RegisterSound( soundID, filepath );
	}
}
