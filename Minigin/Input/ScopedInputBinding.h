#ifndef DAE_SCOPEDINPUTBINDING_H
#define DAE_SCOPEDINPUTBINDING_H

#include <memory>
#include <variant>

#include <Minigin/Input/Gamepad.h>
#include <Minigin/Input/InputManager.h>
#include <Minigin/Input/Keyboard.h>

namespace dae::input
{
	class ICommand;

	class ScopedInputBinding final
	{
	public:
		ScopedInputBinding( Keyboard::Key key, InputManager::KeyState state, std::unique_ptr<ICommand> command );
		ScopedInputBinding( Gamepad::Button button, InputManager::KeyState state, unsigned int controllerIndex, std::unique_ptr<ICommand> command );

		~ScopedInputBinding();

		ScopedInputBinding( const ScopedInputBinding& ) = delete;
		ScopedInputBinding& operator=( const ScopedInputBinding& ) = delete;

		ScopedInputBinding( ScopedInputBinding&& other ) noexcept;
		ScopedInputBinding& operator=( ScopedInputBinding&& other ) noexcept;

	private:
		struct KeyboardTrack
		{
			Keyboard::Key key;
			InputManager::KeyState state;
		};

		struct GamepadTrack
		{
			unsigned int controllerIndex;
			Gamepad::Button button;
			InputManager::KeyState state;
		};

		using BindingTrack = std::variant<std::monostate, KeyboardTrack, GamepadTrack>;
		BindingTrack m_track{ std::monostate{} };

		void Unbind();
	};
}
#endif
