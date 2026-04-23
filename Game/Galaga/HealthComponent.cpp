#include "Game/Galaga/HealthComponent.h"

#include "Minigin/Core/Component.h"
#include "Minigin/Core/SDBMHash.h"
#include "Minigin/Events/GameEvent.h"
#include "Minigin/Events/ObservableComponent.h"

namespace bvi::galaga
{
	HealthComponent::HealthComponent( dae::core::GameObject* owner, int lives )
		: ObservableComponent( owner )
		, m_lives( lives )
	{}

	void HealthComponent::InitializeLinkage()
	{}

	void HealthComponent::InitializeState()
	{
		NotifyPlayerDied();
	}

	void HealthComponent::Update( const float /*deltaTime*/ ) noexcept
	{}

	int HealthComponent::GetLives() const
	{
		return m_lives;
	}

	void HealthComponent::Die()
	{
		if ( m_lives > 0 )
		{
			--m_lives;
			NotifyPlayerDied();
		}
	}

	void HealthComponent::NotifyPlayerDied()
	{
		dae::events::GameEvent event{ dae::core::make_sdbm_hash( "PlayerDied" ) };
		event.PushArg( m_lives );
		NotifyObservers( event );
	}
}
