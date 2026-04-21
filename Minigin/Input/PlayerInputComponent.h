#ifndef PLAYERINPUTCOMPONENT_H
#define PLAYERINPUTCOMPONENT_H

namespace dae::input
{
	class PlayerInputComponent final : public Component
	{
	public:
		void Start() override
		{
			auto* moveComponent = GetGameObject()->GetComponent<MoveComponent>();
			if ( !moveComponent ) return;

			auto moveCmd = std::make_unique<MoveCommand>( moveComponent, glm::vec2{ 1.0f, 0.0f } );

			m_bindings.emplace_back(
				dae::input::InputId{ Gamepad::Button::DpadRight, KeyState::Pressed },
				std::move( moveCmd )
			);
		}

	private:
		std::vector<dae::input::ScopedInputBinding> m_bindings;
	};
}
#endif
