#include <utility>
#include <vector>

#include <Minigin/Core/Component.h>
#include <Minigin/Core/GameObject.h>
#include <Minigin/Input/PlayerInputComponent.h>
#include <Minigin/Input/ScopedInputBinding.h>

namespace dae::input
{
	PlayerInputComponent::PlayerInputComponent( core::GameObject* owner )
		: Component( owner )
	{}

	PlayerInputComponent::~PlayerInputComponent() = default;

	void PlayerInputComponent::InitializeLinkage()
	{}

	void PlayerInputComponent::InitializeState()
	{}

	void PlayerInputComponent::Update( const float /*deltaTime*/ )
	{}

	void PlayerInputComponent::AddBinding( ScopedInputBinding&& binding )
	{
		m_bindings.emplace_back( std::move( binding ) );
	}
}
