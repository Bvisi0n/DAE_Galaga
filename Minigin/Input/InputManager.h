#ifndef DAE_INPUTMANAGER_H
#define DAE_INPUTMANAGER_H

#include <algorithm>
#include <concepts>
#include <memory>
#include <type_traits>
#include <vector>

#include <Minigin/Input/Gamepad.h>
#include <Minigin/Input/ICommand.h>
#include <Minigin/Input/Keyboard.h>
#include <Minigin/Utilities/Singleton.h>

union SDL_Event;

namespace dae::input
{
	template<typename T>
	concept IsInputType = std::same_as<T, Gamepad::Button> || std::same_as<T, Keyboard::Key>;

	class InputManager final : public utils::Singleton<InputManager>
	{
	public:
		enum class KeyState : std::uint8_t
		{
			Down, Up, Pressed
		};

		InputManager() noexcept;
		~InputManager() = default;

		InputManager( const InputManager& ) = delete;
		InputManager( InputManager&& ) = delete;
		InputManager& operator=( const InputManager& ) = delete;
		InputManager& operator=( InputManager&& ) = delete;

		[[nodiscard]] bool ProcessInput( float deltaTime );

		template <IsInputType T>
		void BindCommand( T inputType, KeyState state, std::unique_ptr<ICommand> command, unsigned int controllerIndex = 0 )
		{
			if constexpr ( std::is_same_v<T, Keyboard::Key> )
			{
				auto iterator = std::find_if
				(
					m_keyboardCommands.begin(),
					m_keyboardCommands.end(),
					[ inputType, state ] ( const KeyboardCommandBinding& binding )
					{
						return binding.key == inputType && binding.state == state;
					}
				);

				if ( iterator != m_keyboardCommands.end() )
				{
					iterator->command = std::move( command );
				}
				else
				{
					m_keyboardCommands.push_back( { inputType, state, std::move( command ) } );
				}
			}
			else if constexpr ( std::is_same_v<T, Gamepad::Button> )
			{
				auto iterator = std::find_if
				(
					m_gamepadCommands.begin(),
					m_gamepadCommands.end(),
					[ controllerIndex, inputType, state ] ( const GamepadCommandBinding& binding )
					{
						return binding.controllerIndex == controllerIndex && binding.button == inputType && binding.state == state;
					}
				);

				if ( iterator != m_gamepadCommands.end() )
				{
					iterator->command = std::move( command );
				}
				else
				{
					m_gamepadCommands.push_back( { controllerIndex, inputType, state, std::move( command ) } );
				}
			}
		}

		template <IsInputType T>
		void UnbindCommand( T inputType, KeyState state, unsigned int controllerIndex = 0 )
		{
			if constexpr ( std::is_same_v<T, Keyboard::Key> )
			{
				std::erase_if
				(
					m_keyboardCommands,
					[ inputType, state ] ( const KeyboardCommandBinding& binding )
					{
						return binding.key == inputType && binding.state == state;
					}
				);
			}
			else if constexpr ( std::is_same_v<T, Gamepad::Button> )
			{
				std::erase_if
				(
					m_gamepadCommands,
					[ controllerIndex, inputType, state ] ( const GamepadCommandBinding& binding )
					{
						return binding.controllerIndex == controllerIndex && binding.button == inputType && binding.state == state;
					}
				);
			}
		}

		void ClearAllBinds() noexcept;

		[[nodiscard]] bool IsControllerConnected( unsigned int controllerIndex ) const;

	private:
		struct KeyboardCommandBinding
		{
			Keyboard::Key key;
			KeyState state;
			std::unique_ptr<ICommand> command;
		};

		struct GamepadCommandBinding
		{
			unsigned int controllerIndex;
			Gamepad::Button button;
			KeyState state;
			std::unique_ptr<ICommand> command;
		};

		std::vector<GamepadCommandBinding> m_gamepadCommands;
		std::vector<KeyboardCommandBinding> m_keyboardCommands;

		std::unique_ptr<Keyboard> m_keyboard;
		std::vector<std::unique_ptr<Gamepad>> m_gamepads;

		[[nodiscard]] bool ProcessSDLEvents();
		void UpdateHardwareStates();
		void ExecuteKeyboardCommands( float deltaTime );
		void ExecuteGamepadCommands( float deltaTime );

	#ifndef WIN32
		void RouteSDLHardwareEvent( const SDL_Event& event );
	#endif
	};
}
#endif
