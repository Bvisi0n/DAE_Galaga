#include <cassert>
#include <map>
#include <memory>

#include <SDL3/SDL_events.h>

#include <backends/imgui_impl_sdl3.h>

#include "Minigin/Input/Command.h"
#include "Minigin/Input/Gamepad.h"
#include "Minigin/Input/InputManager.h"
#include "Minigin/Input/Keyboard.h"

namespace dae::input
{
	InputManager::InputManager() noexcept
		: m_keyboard( std::make_unique<Keyboard>() )
	{
		constexpr int maxControllers = 4;
		m_gamepads.reserve( maxControllers );
		for ( unsigned int index = 0; index < maxControllers; ++index )
		{
			m_gamepads.push_back( std::make_unique<Gamepad>( index ) );
		}
	}

	bool InputManager::ProcessInput( const float deltaTime )
	{
		SDL_Event event;
		while ( SDL_PollEvent( &event ) )
		{
			if ( event.type == SDL_EVENT_QUIT )
			{
				return false;
			}

			ImGui_ImplSDL3_ProcessEvent( &event );
		}

		// SDL_PollEvent() must come first.
		// See https://youtu.be/TSlJ3dX5GCI?si=_Nb7xtJYRhaWbBvI&t=339 for info.

		if ( m_keyboard )
		{
			m_keyboard->Update();
		}

		for ( auto& gamepad : m_gamepads )
		{
			if ( gamepad )
			{
				gamepad->Update();
			}
		}

		auto checkState = [] ( KeyState state, auto isDown, auto isUp, auto isPressed ) -> bool
			{
				switch ( state )
				{
					case KeyState::Down:
						return isDown();
					case KeyState::Up:
						return isUp();
					case KeyState::Pressed:
						return isPressed();
					default:
						return false;
				}
			};

		for ( auto& [binding, command] : m_keyboardCommands )
		{
			if ( command == nullptr )
			{
				continue;
			}

			const auto& [key, state] = binding;
			if ( checkState( state,
							 [ & ] { return m_keyboard->IsDown( key ); },
							 [ & ] { return m_keyboard->IsUp( key ); },
							 [ & ] { return m_keyboard->IsPressed( key ); } ) )
			{
				command->Execute( deltaTime );
			}
		}

		for ( auto& [binding, command] : m_gamepadCommands )
		{
			if ( command == nullptr )
			{
				continue;
			}

			const auto& [index, button, state] = binding;

			if ( index >= m_gamepads.size() || m_gamepads[ index ] == nullptr )
			{
				continue;
			}

			if ( checkState( state,
							 [ & ] { return m_gamepads[ index ]->IsDown( button ); },
							 [ & ] { return m_gamepads[ index ]->IsUp( button ); },
							 [ & ] { return m_gamepads[ index ]->IsPressed( button ); } ) )
			{
				command->Execute( deltaTime );
			}
		}

		return true;
	}

	void InputManager::AssertAndRemoveBindings( const void* targetContext )
	{
		if ( !targetContext )
		{
			return;
		}

		auto isDangling = [ targetContext ] ( const auto& pair )
			{
				const std::unique_ptr<Command>& cmd = pair.second;
				if ( cmd && cmd->GetTargetContext() == targetContext )
				{
					assert( false && "Dangling Pointer! Command was not explicitly unbound before target destruction." );
					return true;
				}
				return false;
			};

		std::erase_if( m_gamepadCommands, isDangling );
		std::erase_if( m_keyboardCommands, isDangling );
	}

	bool InputManager::IsControllerConnected( unsigned int controllerIndex ) const
	{
		if ( controllerIndex >= m_gamepads.size() )
		{
			return false;
		}

		return m_gamepads[ controllerIndex ]->IsConnected();
	}
}
