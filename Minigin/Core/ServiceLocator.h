#ifndef DAE_SERVICELOCATOR_H
#define DAE_SERVICELOCATOR_H

#include <memory>

#include "Minigin/Audio/ISoundSystem.h"

namespace dae::core
{
	class ServiceLocator final
	{
	public:
		ServiceLocator() = delete;

		// TODO dae_core - Rule of 5?

		static void RegisterSoundSystem( std::unique_ptr<audio::ISoundSystem>&& soundSystem );
		static audio::ISoundSystem& GetSoundSystem();

	private:
		inline static std::unique_ptr<audio::ISoundSystem> m_soundSystem = std::make_unique<audio::NullSoundSystem>();
	};
}
#endif
