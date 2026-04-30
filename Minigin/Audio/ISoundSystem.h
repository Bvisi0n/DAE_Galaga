#ifndef DAE_ISOUNDSYSTEM_H
#define DAE_ISOUNDSYSTEM_H

#include <cstdint>

// TODO dae_audio - Rework audio system.
	// Move struct PlayMessage to this file, use it as argument.
	// Add RegisterSound method.
	// Figure out an optimized way to manage audio resources.
	// Fix RAII wrappers/deleters, MIX_Quit(), memory leak if not used.
	// Implement in Galaga.

namespace dae::audio
{
	using sound_id = uint16_t;

	class ISoundSystem
	{
	public:
		ISoundSystem() = default;
		virtual ~ISoundSystem() = default;

		ISoundSystem( const ISoundSystem& ) = delete;
		ISoundSystem( ISoundSystem&& ) = default;
		ISoundSystem& operator=( const ISoundSystem& ) = delete;
		ISoundSystem& operator=( ISoundSystem&& ) = default;

		virtual void Play( sound_id id, float volume ) = 0;
	};

	class NullSoundSystem final : public ISoundSystem
	{
	public:
		NullSoundSystem() = default;
		~NullSoundSystem() override = default;

		NullSoundSystem( const NullSoundSystem& ) = delete;
		NullSoundSystem( NullSoundSystem&& ) = default;
		NullSoundSystem& operator=( const NullSoundSystem& ) = delete;
		NullSoundSystem& operator=( NullSoundSystem&& ) = default;

		void Play( sound_id /*id*/, float /*volume*/ ) override
		{}
	};
}
#endif
