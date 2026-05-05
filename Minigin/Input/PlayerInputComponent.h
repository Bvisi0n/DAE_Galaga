#ifndef DAE_PLAYERINPUTCOMPONENT_H
#define DAE_PLAYERINPUTCOMPONENT_H

#include <vector>

#include <Minigin/Core/Component.h>
#include <Minigin/Input/ScopedInputBinding.h>

namespace dae::core
{
	class GameObject;
}

namespace dae::input
{
	class PlayerInputComponent final : public core::Component
	{
	public:
		explicit PlayerInputComponent( core::GameObject* owner );
		~PlayerInputComponent() override;

		PlayerInputComponent( const PlayerInputComponent& ) = delete;
		PlayerInputComponent( PlayerInputComponent&& ) = delete;
		PlayerInputComponent& operator=( const PlayerInputComponent& ) = delete;
		PlayerInputComponent& operator=( PlayerInputComponent&& ) = delete;

		void InitializeLinkage() override;
		void InitializeState() override;

		void Update( float /*deltaTime*/ ) override;
		void AddBinding( ScopedInputBinding&& binding );

	private:
		std::vector<ScopedInputBinding> m_bindings;
	};
}
#endif
