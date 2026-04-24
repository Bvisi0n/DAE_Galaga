#ifndef PLAYERINPUTCOMPONENT_H
#define PLAYERINPUTCOMPONENT_H

#include <vector>

#include "Minigin/Core/Component.h"
#include "Minigin/Core/GameObject.h"
#include "Minigin/Input/ScopedInputBinding.h"

// TODO dae_input - Incomplete & unused class.

namespace dae::input
{
	class PlayerInputComponent final : public Component
	{
	public:
		explicit PlayerInputComponent( GameObject* owner )
			: Component( owner )
		{}

		~PlayerInputComponent() override = default;

		PlayerInputComponent( const PlayerInputComponent& ) = delete;
		PlayerInputComponent( PlayerInputComponent&& ) = delete;
		PlayerInputComponent& operator=( const PlayerInputComponent& ) = delete;
		PlayerInputComponent& operator=( PlayerInputComponent&& ) = delete;

		void InitializeLinkage() override
		{}

		void InitializeState() override
		{}

		void Update( const float /*deltaTime*/ ) override
		{}

	private:
		std::vector<dae::input::ScopedInputBinding> m_bindings{};
	};
}
#endif
