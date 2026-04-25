#ifndef DAE_PLAYERINPUTCOMPONENT_H
#define DAE_PLAYERINPUTCOMPONENT_H

#include <utility>
#include <vector>

#include "Minigin/Core/Component.h"
#include "Minigin/Core/GameObject.h"
#include "Minigin/Input/ScopedInputBinding.h"

namespace dae::input
{
	class PlayerInputComponent final : public core::Component
	{
	public:
		explicit PlayerInputComponent( core::GameObject* owner )
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

		void AddBinding( ScopedInputBinding&& binding )
		{
			m_bindings.emplace_back( std::move( binding ) );
		}

	private:
		std::vector<ScopedInputBinding> m_bindings{};
	};
}
#endif
