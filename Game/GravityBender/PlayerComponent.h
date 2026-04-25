#ifndef BVI_PLAYERCOMPONENT_H
#define BVI_PLAYERCOMPONENT_H

#include <vector>

#include "Minigin/Core/Component.h"
#include "Minigin/Core/GameObject.h"
#include "Minigin/Input/ScopedInputBinding.h"

namespace bvi::gravity_bender
{
	class PlayerComponent final : public dae::core::Component
	{
	public:
		explicit PlayerComponent( dae::core::GameObject* owner );
		~PlayerComponent() override;

		PlayerComponent( const PlayerComponent& other ) = delete;
		PlayerComponent( PlayerComponent&& other ) = delete;
		PlayerComponent& operator=( const PlayerComponent& other ) = delete;
		PlayerComponent& operator=( PlayerComponent&& other ) = delete;

		void InitializeLinkage() override;
		void InitializeState() override;

		void Update( const float /*deltaTime*/ ) override;

	private:
		// TODO bvi_gravity_bender - Should be using PlayerInputComponent.
		std::vector<dae::input::ScopedInputBinding> m_bindings;
	};
}
#endif