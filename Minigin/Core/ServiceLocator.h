#ifndef DAE_SERVICELOCATOR_H
#define DAE_SERVICELOCATOR_H

#include <memory>

#include <Minigin/Audio/ISoundSystem.h>

// https://gameprogrammingpatterns.com/service-locator.html
// C.4: Make a function a member only if it needs direct access to the representation of a class
// SF.20: Use namespaces to express logical structure

namespace dae::core::service_locator
{
	void RegisterSoundSystem( std::unique_ptr<audio::ISoundSystem>&& soundSystem );
	[[nodiscard]] audio::ISoundSystem& GetSoundSystem();
}
#endif
