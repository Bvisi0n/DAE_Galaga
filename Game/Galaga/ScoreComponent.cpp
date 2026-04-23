#include "Game/Galaga/ScoreComponent.h"

#include "Minigin/Core/GameObject.h"
#include "Minigin/Core/SDBMHash.h"
#include "Minigin/Events/GameEvent.h"
#include "Minigin/Events/ObservableComponent.h"

namespace bvi::galaga
{
	ScoreComponent::ScoreComponent( dae::core::GameObject* owner )
		: ObservableComponent( owner )
		, m_score( 0 )
	{}

	void ScoreComponent::InitializeLinkage() noexcept
	{}

	void ScoreComponent::InitializeState() noexcept
	{
		NotifyScoreChanged();
	}

	void ScoreComponent::Update( const float /*deltaTime*/ ) noexcept
	{}

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
