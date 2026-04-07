#ifndef SCORECOMMAND_H
#define SCORECOMMAND_H

#include "Game/Components/ScoreComponent.h"

#include "Minigin/Input/Command.h"

namespace bvi
{
	class ScoreCommand final : public dae::input::Command
	{
	public:
		ScoreCommand( ScoreComponent* scoreComponent, int valueOfChange )
			: m_scoreComponent( scoreComponent )
			, m_valueOfChange( valueOfChange )
		{}

		void Execute( const float ) override
		{
			m_scoreComponent->AddScore( m_valueOfChange );
		}

	private:
		ScoreComponent* m_scoreComponent;
		int m_valueOfChange{};
	};
}
#endif
