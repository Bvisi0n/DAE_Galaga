#include <atomic>
#include <cassert>
#include <condition_variable>
#include <memory>
#include <mutex>
#include <queue>
#include <thread>
#include <vector>

#include <SDL3/SDL.h>
#include <SDL3_mixer/SDL_mixer.h>

#include "ISoundSystem.h"
#include "Minigin/Audio/SDLSoundSystem.h"

namespace dae::audio
{
	struct PlayMessage
	{
		sound_id id;
		float volume;
	};

	class SDLSoundSystem::SDLSoundSystemImpl
	{
	public:
		SDLSoundSystemImpl()
		{
			// TODO dae_audio - Initialize SDL_mixer here

			m_workerThread = std::jthread( &SDLSoundSystemImpl::ProcessQueue, this );
		}

		~SDLSoundSystemImpl()
		{
			m_isAlive.store( false );
			m_queueCondition.notify_one();

			// TODO dae_audio - Cleanup SDL_mixer here
		}

		void Play( sound_id id, float volume )
		{
			std::lock_guard<std::mutex> lock( m_queueMutex );
			m_queue.push( { id, volume } );
			m_queueCondition.notify_one();
		}

	private:
		void ProcessQueue()
		{
			while ( m_isAlive.load() )
			{
				PlayMessage currentMessage{};

				{
					std::unique_lock<std::mutex> lock( m_queueMutex );

					m_queueCondition.wait( lock,
										  [ this ] ()
										  {
											  return !m_queue.empty() || !m_isAlive.load();
										  } );

					if ( !m_queue.empty() )
					{
						currentMessage = m_queue.front();
						m_queue.pop();
					}
				}

				if ( currentMessage.id != 0 )
				{
					// TODO dae_audio - Execute SDL_mixer Play logic here
				}
			}
		}

		std::queue<PlayMessage> m_queue;
		std::mutex m_queueMutex;
		std::condition_variable m_queueCondition;
		std::jthread m_workerThread;
		std::atomic<bool> m_isAlive{ true };
	};

	SDLSoundSystem::SDLSoundSystem() : m_pimpl( std::make_unique<SDLSoundSystemImpl>() )
	{}

	SDLSoundSystem::~SDLSoundSystem() = default;

	void SDLSoundSystem::Play( sound_id id, float volume )
	{
		m_pimpl->Play( id, volume );
	}
}
