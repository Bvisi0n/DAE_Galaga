#if WIN32
#define WIN32_LEAN_AND_MEAN
#include <windows.h> // Required before XInput.h
#include <XInput.h>
#else
#include <SDL3/SDL.h>
#endif

#include <memory>

#include "Minigin/Input/Gamepad.h"

// TODO dae_input - Rework the SDL implementation.
	// XInput was made first and works as intended, SDL was then butchered to match it.
	// A result of this is a flawed controller identification logic.
	// SDL is event driven, currently burning cycles for no reason simulating XInput.

namespace dae::input
{
	class Gamepad::GamepadImpl
	{
	#if WIN32
	public:
		GamepadImpl( unsigned int index ) : m_controllerIndex( index )
		{
			ZeroMemory( &m_currentState, sizeof( XINPUT_STATE ) );
			ZeroMemory( &m_previousState, sizeof( XINPUT_STATE ) );
		}

		void Update()
		{
			m_previousState = m_currentState;
			ZeroMemory( &m_currentState, sizeof( XINPUT_STATE ) );
			DWORD result = XInputGetState( m_controllerIndex, &m_currentState );

			if ( result != ERROR_SUCCESS )
			{
				ZeroMemory( &m_currentState, sizeof( XINPUT_STATE ) );
				ZeroMemory( &m_previousState, sizeof( XINPUT_STATE ) );
				m_buttonsPressedThisFrame = 0;
				m_buttonsReleasedThisFrame = 0;
				m_isConnected = false;
				return;
			}

			m_isConnected = true;

			auto button_changes = m_currentState.Gamepad.wButtons ^ m_previousState.Gamepad.wButtons;
			m_buttonsPressedThisFrame = button_changes & m_currentState.Gamepad.wButtons;
			m_buttonsReleasedThisFrame = button_changes & ( ~m_currentState.Gamepad.wButtons );
		}

		bool IsDown( Gamepad::Button button ) const
		{
			return m_buttonsPressedThisFrame & static_cast<unsigned int>( button );
		}

		bool IsUp( Gamepad::Button button ) const
		{
			return m_buttonsReleasedThisFrame & static_cast<unsigned int>( button );
		}

		bool IsPressed( Gamepad::Button button ) const
		{
			return m_currentState.Gamepad.wButtons & static_cast<unsigned int>( button );
		}

		bool IsConnected() const
		{
			return m_isConnected;
		}

	private:
		unsigned int m_controllerIndex;
		XINPUT_STATE m_currentState{};
		XINPUT_STATE m_previousState{};
		unsigned int m_buttonsPressedThisFrame{};
		unsigned int m_buttonsReleasedThisFrame{};
		bool m_isConnected{};
	#else
	public:
		explicit GamepadImpl( unsigned int controllerIndex )
			: m_controllerIndex( controllerIndex )
		{}

		~GamepadImpl()
		{
			if ( m_gamepad ) SDL_CloseGamepad( m_gamepad );
		}

		void Update()
		{
			if ( !m_gamepad )
			{
				int count{};
				std::unique_ptr<SDL_JoystickID, void( * )( void* )> gamepads( SDL_GetGamepads( &count ), SDL_free );
				if ( gamepads && m_controllerIndex < static_cast<unsigned int>( count ) )
				{
					m_gamepad = SDL_OpenGamepad( gamepads.get()[ m_controllerIndex ] );
				}
			}

			if ( !m_gamepad )
			{
				return;
			}

			m_previousButtons = m_currentButtons;
			m_currentButtons = 0;

			struct ButtonMapping
			{
				SDL_GamepadButton sdlButton;
				Gamepad::Button daeButton;
			};

			// https://wiki.libsdl.org/SDL3/SDL_GamepadButton
			static constexpr ButtonMapping mapping[] = {
				{ SDL_GAMEPAD_BUTTON_SOUTH, Button::A },
				{ SDL_GAMEPAD_BUTTON_EAST, Button::B },
				{ SDL_GAMEPAD_BUTTON_WEST, Button::X },
				{ SDL_GAMEPAD_BUTTON_NORTH, Button::Y },
				{ SDL_GAMEPAD_BUTTON_START, Button::Start },
				{ SDL_GAMEPAD_BUTTON_BACK, Button::Back },
				{ SDL_GAMEPAD_BUTTON_LEFT_STICK, Button::LeftThumb },
				{ SDL_GAMEPAD_BUTTON_RIGHT_STICK, Button::RightThumb },
				{ SDL_GAMEPAD_BUTTON_LEFT_SHOULDER, Button::LeftShoulder },
				{ SDL_GAMEPAD_BUTTON_RIGHT_SHOULDER, Button::RightShoulder },
				{ SDL_GAMEPAD_BUTTON_DPAD_UP, Button::DPadUp },
				{ SDL_GAMEPAD_BUTTON_DPAD_DOWN, Button::DPadDown },
				{ SDL_GAMEPAD_BUTTON_DPAD_LEFT, Button::DPadLeft },
				{ SDL_GAMEPAD_BUTTON_DPAD_RIGHT, Button::DPadRight }
			};

			for ( const auto& m : mapping )
			{
				if ( SDL_GetGamepadButton( m_gamepad, m.sdlButton ) )
				{
					m_currentButtons |= static_cast<unsigned int>( m.daeButton );
				}
			}
		}

		bool IsDown( Gamepad::Button button ) const
		{
			return ( m_currentButtons & static_cast<unsigned int>( button ) ) && !( m_previousButtons & static_cast<unsigned int>( button ) );
		}

		bool IsUp( Gamepad::Button button ) const
		{
			return !( m_currentButtons & static_cast<unsigned int>( button ) ) && ( m_previousButtons & static_cast<unsigned int>( button ) );
		}

		bool IsPressed( Gamepad::Button button ) const
		{
			return m_currentButtons & static_cast<unsigned int>( button );
		}

		bool IsConnected() const
		{
			return m_gamepad != nullptr;
		}

	private:
		unsigned int m_controllerIndex;
		SDL_Gamepad* m_gamepad{ nullptr };
		unsigned int m_currentButtons{};
		unsigned int m_previousButtons{};
	#endif
	};

	Gamepad::Gamepad( unsigned int controllerIndex )
		: m_pimpl( std::make_unique<GamepadImpl>( controllerIndex ) )
	{}

	Gamepad::~Gamepad() = default;

	void Gamepad::Update()
	{
		m_pimpl->Update();
	}

	bool Gamepad::IsDown( Button button ) const
	{
		return m_pimpl->IsDown( button );
	}

	bool Gamepad::IsUp( Button button ) const
	{
		return m_pimpl->IsUp( button );
	}

	bool Gamepad::IsPressed( Button button ) const
	{
		return m_pimpl->IsPressed( button );
	}

	bool Gamepad::IsConnected() const
	{
		return m_pimpl->IsConnected();
	}
}
