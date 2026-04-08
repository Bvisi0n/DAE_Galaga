#include "Game/Components/ScoreComponent.h"

#include "Minigin/Core/GameObject.h"
#include "Minigin/Core/SDBMHash.h"
#include "Minigin/Events/GameEvent.h"
#include "Minigin/Events/ObservableComponent.h"
#include "Minigin/Input/InputManager.h"

namespace bvi::components
{
	ScoreComponent::ScoreComponent( dae::core::GameObject* owner )
		: ObservableComponent( owner )
		, m_score( 0 )
	{}

	ScoreComponent::~ScoreComponent()
	{
		// TODO L: Should we bake this in to a base class or rely on the dev placing this wherever required?
		dae::input::InputManager::GetInstance().AssertAndRemoveBindings( this );
	}

	void ScoreComponent::InitializeState() noexcept
	{
		NotifyScoreChanged();
	}

	void ScoreComponent::AddScore( const int score )
	{
		m_score += score;
		NotifyScoreChanged();
	}

	int ScoreComponent::GetScore() const
	{
		return m_score;
	}

	void ScoreComponent::NotifyScoreChanged()
	{
		dae::events::GameEvent event{ dae::core::make_sdbm_hash( "ScoreChanged" ) };
		event.PushArg( m_score );
		NotifyObservers( event );
	}
}
