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

// SDL3_mixer documentation: https://wiki.libsdl.org/SDL3_mixer/CategorySDLMixer

namespace dae::audio
{
	struct PlayMessage
	{
		sound_id id;
		float volume;
	};

	struct MixerDeleter
	{
		void operator()( MIX_Mixer* mixer ) const
		{
			if ( mixer )
			{
				MIX_DestroyMixer( mixer ); // TODO dae_audio - Exception!
			}
		}
	};

	struct AudioDeleter
	{
		void operator()( MIX_Audio* audio ) const
		{
			if ( audio )
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

		bool IsLoaded() const
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

			m_mixer.reset( MIX_CreateMixerDevice( SDL_AUDIO_DEVICE_DEFAULT_PLAYBACK, 0 ) );
			if ( !m_mixer )
			{
				assert( false && "Failed to create SDL3_mixer device" );
				return;
			}

			m_tracks.reserve( 16 );
			for ( int i = 0; i < 16; ++i )
			{
				m_tracks.push_back( MIX_CreateTrack( m_mixer.get() ) );
			}

			m_workerThread = std::jthread( [ this ] ( std::stop_token st )
										   {
											   ProcessQueue( st );
										   } );
		}

		~SDLSoundSystemImpl()
		{
			m_workerThread.request_stop();
			if ( m_workerThread.joinable() )
			{
				m_workerThread.join();
			}

			if ( m_mixer )
			{
				for ( MIX_Track* track : m_tracks )
				{
					MIX_StopTrack( track, 0 );
					MIX_SetTrackAudio( track, nullptr );
				}
			}

			m_tracks.clear();
			m_audioCache.clear();
			m_mixer.reset();

			MIX_Quit(); // TODO dae_audio - Exception!
		}

		void Play( sound_id id, float volume )
		{
			std::lock_guard<std::mutex> lock( m_queueMutex );
			m_queue.push( { id, volume } );
			m_queueCondition.notify_one();
		}

		void RegisterSound( sound_id id, const std::filesystem::path& filepath )
		{
			if ( id >= m_audioCache.size() )
			{
				assert( id < std::numeric_limits<sound_id>::max() && "Sound ID exceeds maximum value" );
				m_audioCache.resize( id + 1 );
			}
			m_audioCache[ id ].filepath = filepath;
		}

	private:
		UniqueMixer m_mixer;
		std::vector<MIX_Track*> m_tracks;
		std::vector<AudioClip> m_audioCache;

		std::queue<PlayMessage> m_queue;
		std::mutex m_queueMutex;
		std::condition_variable_any m_queueCondition;

		std::jthread m_workerThread;

		void ProcessQueue( std::stop_token stopToken )
		{
			while ( !stopToken.stop_requested() )
			{
				PlayMessage currentMessage{};

				{
					std::unique_lock<std::mutex> lock( m_queueMutex );
					m_queueCondition.wait( lock, stopToken, [ this ] () {
						return !m_queue.empty();
					} );

					if ( stopToken.stop_requested() && m_queue.empty() ) break;
					if ( m_queue.empty() ) continue;

					currentMessage = m_queue.front();
					m_queue.pop();
				}

				if ( currentMessage.id < m_audioCache.size() )
				{
					AudioClip& clip = m_audioCache[ currentMessage.id ];

					if ( !clip.IsLoaded() )
					{
						clip.audio.reset( MIX_LoadAudio( m_mixer.get(), clip.filepath.string().c_str(), true ) );
						if ( !clip.IsLoaded() ) continue;
					}

					MIX_Track* availableTrack = nullptr;
					for ( MIX_Track* track : m_tracks )
					{
						if ( !MIX_TrackPlaying( track ) )
						{
							availableTrack = track;
							break;
						}
					}

					if ( availableTrack )
					{
						MIX_SetTrackAudio( availableTrack, clip.audio.get() );
						MIX_SetTrackGain( availableTrack, currentMessage.volume );
						MIX_PlayTrack( availableTrack, 0 );
					}
					else
					{
						// Discard request.
					}
				}
			}
		}
	};

	SDLSoundSystem::SDLSoundSystem() : m_pimpl( std::make_unique<SDLSoundSystemImpl>() )
	{}

	SDLSoundSystem::~SDLSoundSystem() = default;

	void SDLSoundSystem::Play( sound_id id, float volume )
	{
		m_pimpl->Play( id, volume );
	}
}
