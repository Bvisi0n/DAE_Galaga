#include <memory>

#include <SDL3/SDL_events.h>

#include <backends/imgui_impl_sdl3.h>

#include <Minigin/Input/Gamepad.h>
#include <Minigin/Input/ICommand.h>
#include <Minigin/Input/InputManager.h>
#include <Minigin/Input/Keyboard.h>

namespace dae::input
{
	InputManager::InputManager() noexcept
		: m_keyboard( std::make_unique<Keyboard>() )
	{
		constexpr unsigned int maxControllers = 4;
		m_gamepads.reserve( maxControllers );

		for ( unsigned int index = 0; index < maxControllers; ++index )
		{
			m_gamepads.push_back( std::make_unique<Gamepad>( index ) );
		}
	}

	bool InputManager::ProcessInput( const float deltaTime )
	{
		if ( !ProcessSDLEvents() )
		{
			return false;
		}

		UpdateHardwareStates();
		ExecuteKeyboardCommands( deltaTime );
		ExecuteGamepadCommands( deltaTime );

		return true;
	}

	void InputManager::ClearAllBinds() noexcept
	{
		m_keyboardCommands.clear();
		m_gamepadCommands.clear();
	}

	bool InputManager::IsControllerConnected( unsigned int controllerIndex ) const
	{
		if ( controllerIndex >= m_gamepads.size() || m_gamepads[ controllerIndex ] == nullptr )
		{
			return false;
		}

		return m_gamepads[ controllerIndex ]->IsConnected();
	}

	bool InputManager::ProcessSDLEvents()
	{
		SDL_Event event;
		while ( SDL_PollEvent( &event ) )
		{
			if ( event.type == SDL_EVENT_QUIT )
			{
				return false;
			}

			ImGui_ImplSDL3_ProcessEvent( &event );

		#ifndef WIN32
			RouteSDLHardwareEvent( event );
		#endif
		}

		return true;
	}

	void InputManager::UpdateHardwareStates()
	{
		if ( m_keyboard != nullptr )
		{
			m_keyboard->Update();
		}

		for ( const auto& gamepad : m_gamepads )
		{
			if ( gamepad != nullptr )
			{
				gamepad->Update();
			}
		}
	}

	void InputManager::ExecuteKeyboardCommands( const float deltaTime )
	{
		for ( const auto& binding : m_keyboardCommands )
		{
			if ( binding.command == nullptr )
			{
				continue;
			}

			bool isTriggered = false;
			switch ( binding.state )
			{
				case KeyState::Down:
					isTriggered = m_keyboard->IsDown( binding.key );
					break;
				case KeyState::Up:
					isTriggered = m_keyboard->IsUp( binding.key );
					break;
				case KeyState::Pressed:
					isTriggered = m_keyboard->IsPressed( binding.key );
					break;
			}

			if ( isTriggered )
			{
				binding.command->Execute( deltaTime );
			}
		}
	}

	void InputManager::ExecuteGamepadCommands( const float deltaTime )
	{
		for ( const auto& binding : m_gamepadCommands )
		{
			if ( binding.command == nullptr )
			{
				continue;
			}

			if ( binding.controllerIndex >= m_gamepads.size() || m_gamepads[ binding.controllerIndex ] == nullptr )
			{
				continue;
			}

			bool isTriggered = false;
			const auto& gamepad = m_gamepads[ binding.controllerIndex ];

			switch ( binding.state )
			{
				case KeyState::Down:
					isTriggered = gamepad->IsDown( binding.button );
					break;
				case KeyState::Up:
					isTriggered = gamepad->IsUp( binding.button );
					break;
				case KeyState::Pressed:
					isTriggered = gamepad->IsPressed( binding.button );
					break;
			}

			if ( isTriggered )
			{
				binding.command->Execute( deltaTime );
			}
		}
	}

#ifndef WIN32
	void InputManager::RouteSDLHardwareEvent( const SDL_Event& event )
	{
		if ( event.type == SDL_EVENT_GAMEPAD_ADDED )
		{
			for ( const auto& gamepad : m_gamepads )
			{
				if ( gamepad != nullptr && !gamepad->IsConnected() )
				{
					gamepad->Open( event.gdevice.which );
					break;
				}
			}
		}
		else if ( event.type == SDL_EVENT_GAMEPAD_REMOVED )
		{
			for ( const auto& gamepad : m_gamepads )
			{
				if ( gamepad != nullptr && gamepad->HasJoystickID( event.gdevice.which ) )
				{
					gamepad->Close();
					break;
				}
			}
		}
		else if ( event.type == SDL_EVENT_GAMEPAD_BUTTON_DOWN || event.type == SDL_EVENT_GAMEPAD_BUTTON_UP )
		{
			for ( const auto& gamepad : m_gamepads )
			{
				if ( gamepad != nullptr && gamepad->HasJoystickID( event.gbutton.which ) )
				{
					gamepad->RegisterButtonEvent( event.gbutton.button, event.type == SDL_EVENT_GAMEPAD_BUTTON_DOWN );
					break;
				}
			}
		}
	}
#endif
}
