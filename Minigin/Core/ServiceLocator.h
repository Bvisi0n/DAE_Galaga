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

		static void RegisterSoundSystem( std::unique_ptr<audio::ISoundSystem>&& system );
		static audio::ISoundSystem& GetSoundSystem();

	private:
		inline static std::unique_ptr<audio::ISoundSystem> m_soundSystem = std::make_unique<audio::NullSoundSystem>();
	};
}
#endif
