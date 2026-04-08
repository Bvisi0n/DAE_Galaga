#ifndef HEALTHCOMPONENT_H
#define HEALTHCOMPONENT_H

#include "Minigin/Events/ObservableComponent.h"

namespace dae::core
{
	class GameObject;
}

namespace bvi::components
{
	class HealthComponent final : public dae::events::ObservableComponent
	{
	public:
		HealthComponent( dae::core::GameObject* owner, int lives );
		~HealthComponent();

		void InitializeLinkage() override
		{}

		void InitializeState() override;

		void Update( const float ) noexcept override
		{}

		[[nodiscard]] int GetLives() const;

		void Die();

	private:
		int m_lives;

		void NotifyPlayerDied();
	};
}
#endif
