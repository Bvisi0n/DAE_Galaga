#include <memory>
#include <utility>

#include <Minigin/Audio/ISoundSystem.h>
#include "Minigin/Core/ServiceLocator.h"

namespace dae::core
{
	void ServiceLocator::RegisterSoundSystem( std::unique_ptr<audio::ISoundSystem>&& soundSystem )
	{
		m_soundSystem = soundSystem ? std::move( soundSystem ) : std::make_unique<audio::NullSoundSystem>();
	}

	audio::ISoundSystem& ServiceLocator::GetSoundSystem()
	{
		return *m_soundSystem;
	}
}
