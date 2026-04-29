#ifndef DAE_SDLSOUNDSYSTEM_H
#define DAE_SDLSOUNDSYSTEM_H

#include <memory>

#include "Minigin/Audio/ISoundSystem.h"

namespace dae::audio
{
	class SDLSoundSystem : public ISoundSystem
	{
	public:
		SDLSoundSystem();
		virtual ~SDLSoundSystem() override;

		SDLSoundSystem( const SDLSoundSystem& ) = delete;
		SDLSoundSystem( SDLSoundSystem&& ) = default;
		SDLSoundSystem& operator=( const SDLSoundSystem& ) = delete;
		SDLSoundSystem& operator=( SDLSoundSystem&& ) = default;

		virtual void Play( sound_id id, float volume ) override;

	private:
		class SDLSoundSystemImpl;
		std::unique_ptr<SDLSoundSystemImpl> m_pimpl;
	};
}
#endif
