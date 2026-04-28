#ifndef DAE_ISOUNDSYSTEM_H
#define DAE_ISOUNDSYSTEM_H

#include <cstdint>

using sound_id = uint16_t;

namespace dae::audio
{
	class ISoundSystem
	{
	public:
		virtual ~ISoundSystem() = default;
		virtual void Play( sound_id id, float volume ) = 0;
	};

	class NullSoundSystem final : public ISoundSystem
	{
	public:
		void Play( sound_id /*id*/, float /*volume*/ ) override
		{}
	};
}
#endif
