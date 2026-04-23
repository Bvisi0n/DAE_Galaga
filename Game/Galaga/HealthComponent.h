#ifndef HEALTHCOMPONENT_H
#define HEALTHCOMPONENT_H

#include "Minigin/Events/ObservableComponent.h"

namespace dae::core
{
	class GameObject;
}

namespace bvi::galaga
{
	class HealthComponent final : public dae::events::ObservableComponent
	{
	public:
		explicit HealthComponent( dae::core::GameObject* owner, int lives );
		~HealthComponent() override = default;

		HealthComponent( const HealthComponent& other ) = delete;
		HealthComponent( HealthComponent&& other ) = delete;
		HealthComponent& operator=( const HealthComponent& other ) = delete;
		HealthComponent& operator=( HealthComponent&& other ) = delete;

		void InitializeLinkage() override;
		void InitializeState() override;

		void Update( const float deltaTime ) noexcept override;

		[[nodiscard]] int GetLives() const;

		void Die();

	private:
		int m_lives;

		void NotifyPlayerDied();
	};
}
#endif
