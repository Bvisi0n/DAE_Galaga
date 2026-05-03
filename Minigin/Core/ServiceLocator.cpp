#include <memory>
#include <utility>

#include <Minigin/Audio/ISoundSystem.h>
#include <Minigin/Core/ServiceLocator.h>

namespace dae::core::service_locator
{
	namespace
	{
		std::unique_ptr<audio::ISoundSystem>& GetInternalSoundSystem()
		{
			static std::unique_ptr<audio::ISoundSystem> instance
			{
				std::make_unique<audio::NullSoundSystem>()
			};

			return instance;
		}
	}

	void RegisterSoundSystem( std::unique_ptr<audio::ISoundSystem>&& soundSystem )
	{
		GetInternalSoundSystem() = soundSystem ? std::move( soundSystem ) : std::make_unique<audio::NullSoundSystem>();
	}

	audio::ISoundSystem& GetSoundSystem()
	{
		return *GetInternalSoundSystem();
	}
}
