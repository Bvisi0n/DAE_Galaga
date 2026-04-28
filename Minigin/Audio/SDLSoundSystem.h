#ifndef DAE_SDLSOUNDSYSTEM_H
#define DAE_SDLSOUNDSYSTEM_H

#include <memory>

#include "Minigin/Audio/ISoundSystem.h"

namespace dae::audio
{
	class SDLSoundSystem final : public ISoundSystem
	{
	public:
		SDLSoundSystem();
		~SDLSoundSystem() override;

		SDLSoundSystem( const SDLSoundSystem& ) = delete;
		SDLSoundSystem& operator=( const SDLSoundSystem& ) = delete;
		SDLSoundSystem( SDLSoundSystem&& ) = default;
		SDLSoundSystem& operator=( SDLSoundSystem&& ) = default;

		void Play( sound_id id, float volume ) override;

	private:
		class SDLSoundSystemImpl;
		std::unique_ptr<SDLSoundSystemImpl> m_pimpl;
	};
}
#endif
