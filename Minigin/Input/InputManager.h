#ifndef INPUTMANAGER_H
#define INPUTMANAGER_H

#include <algorithm>
#include <concepts>
#include <map>
#include <memory>
#include <type_traits>
#include <utility>
#include <vector>

#include "Minigin/Input/Command.h"
#include "Minigin/Input/Gamepad.h"
#include "Minigin/Input/Keyboard.h"
#include "Minigin/Utilities/Singleton.h"

namespace dae::input
{
	template<typename T>
	concept IsInputType = std::same_as<T, Gamepad::Button> || std::same_as<T, Keyboard::Key>;

	class InputManager final : public utils::Singleton<InputManager>
	{
	public:
		enum class KeyState
		{
			Down, Up, Pressed
		};
		InputManager() noexcept;
		~InputManager() = default;

		[[nodiscard]] bool ProcessInput( const float deltaTime );

		template <IsInputType T>
		void BindCommand( T inputType, KeyState state, std::unique_ptr<Command> command, unsigned int controllerIndex = 0 )
		{
			if constexpr ( std::is_same_v<T, Keyboard::Key> )
			{
				m_keyboardCommands[ {inputType, state} ] = std::move( command );
			}
			else if constexpr ( std::is_same_v<T, Gamepad::Button> )
			{
				m_gamepadCommands[ {controllerIndex, inputType, state} ] = std::move( command );
			}
		}

		template <IsInputType T>
		void UnbindCommand( T inputType, KeyState state, unsigned int controllerIndex = 0 )
		{
			if constexpr ( std::is_same_v<T, Keyboard::Key> )
			{
				m_keyboardCommands.erase( { inputType, state } );
			}
			else if constexpr ( std::is_same_v<T, Gamepad::Button> )
			{
				m_gamepadCommands.erase( { controllerIndex, inputType, state } );
			}
		}

		[[nodiscard]] bool IsControllerConnected( unsigned int controllerIndex ) const;

	private:
		struct ControllerKey
		{
			unsigned int controllerIndex;
			Gamepad::Button button;
		};

		using ControllerBinding = std::tuple<unsigned int, Gamepad::Button, KeyState>;
		using KeyboardBinding = std::pair<Keyboard::Key, KeyState>;

		// TODO MINI: Use std::vector and Binding struct?
			// Data locality, cache misses, 1 command per button limit.
		std::map<ControllerBinding, std::unique_ptr<Command>> m_gamepadCommands;
		std::map<KeyboardBinding, std::unique_ptr<Command>> m_keyboardCommands;

		std::unique_ptr<Keyboard> m_keyboard;
		std::vector<std::unique_ptr<Gamepad>> m_gamepads;
	};
}
#endif
