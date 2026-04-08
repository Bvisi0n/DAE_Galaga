#ifndef DIECOMMAND_H
#define DIECOMMAND_H

#include "Game/Components/HealthComponent.h"

#include "Minigin/Input/Command.h"

namespace bvi::commands
{
	class DieCommand final : public dae::input::Command
	{
	public:
		DieCommand( HealthComponent* healthComponent ) noexcept
			: m_healthComponent( healthComponent )
		{}

		void Execute( const float ) override
		{
			m_healthComponent->Die();
		}

		[[nodiscard]] const void* GetTargetContext() const override
		{
			return m_healthComponent;
		}

	private:
		HealthComponent* m_healthComponent;
	};
}
#endif
