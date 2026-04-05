#include "Game/Components/HealthComponent.h"

#include "Minigin/Core/SDBMHash.h"
#include "Minigin/Events/GameEvent.h"

namespace bvi
{
	HealthComponent::HealthComponent(dae::core::GameObject* pOwner, int lives)
		: ObservableComponent(pOwner)
		, m_lives(lives) {}

	void HealthComponent::InitializeState()
	{
		NotifyPlayerDied();
	}

	int HealthComponent::GetLives() const
	{
		return m_lives;
	}

	void HealthComponent::Die()
	{
		if (m_lives > 0)
		{
			--m_lives;
			NotifyPlayerDied();
		}
	}

	void HealthComponent::NotifyPlayerDied() const
	{
		dae::events::GameEvent event{ dae::core::make_sdbm_hash("PlayerDied") };
		event.PushArg(m_lives);
		NotifyObservers(event);
	}
}
