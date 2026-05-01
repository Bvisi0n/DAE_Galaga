#ifndef DAE_SDLSOUNDSYSTEM_H
#define DAE_SDLSOUNDSYSTEM_H

#include <filesystem>
#include <memory>

#include <Minigin/Audio/ISoundSystem.h>

namespace dae::audio
{
	class SDLSoundSystem final : public ISoundSystem
	{
	public:
		SDLSoundSystem();
		~SDLSoundSystem() override;

		SDLSoundSystem( const SDLSoundSystem& ) = delete;
		SDLSoundSystem( SDLSoundSystem&& ) = default;
		SDLSoundSystem& operator=( const SDLSoundSystem& ) = delete;
		SDLSoundSystem& operator=( SDLSoundSystem&& ) = default;

		void Play( const PlayMessage& message ) override;
		void RegisterSound( SoundID soundID, const std::filesystem::path& filepath ) override;

	private:
		class SDLSoundSystemImpl;
		std::unique_ptr<SDLSoundSystemImpl> m_pimpl;
	};
}
#endif
