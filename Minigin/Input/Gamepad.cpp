#if WIN32
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <XInput.h>
#else
#include <SDL3/SDL.h>
#include <array>
#endif

#include <memory>
#include <Minigin/Input/Gamepad.h>

namespace dae::input
{
	class Gamepad::GamepadImpl
	{
	public:
		explicit GamepadImpl( unsigned int controllerIndex )
			: m_controllerIndex( controllerIndex )
		{
		#if WIN32
			ZeroMemory( &m_currentState, sizeof( XINPUT_STATE ) );
			ZeroMemory( &m_previousState, sizeof( XINPUT_STATE ) );
		#endif
		}

	#if WIN32
		~GamepadImpl() = default;
	#else
		~GamepadImpl()
		{
			Close();
		}
	#endif

		GamepadImpl( const GamepadImpl& ) = delete;
		GamepadImpl( GamepadImpl&& ) = delete;
		GamepadImpl& operator=( const GamepadImpl& ) = delete;
		GamepadImpl& operator=( GamepadImpl&& ) = delete;

		void Update()
		{
		#if WIN32
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
		#else
			m_previousButtons = m_currentButtons;
		#endif
		}

		[[nodiscard]] bool IsDown( Gamepad::Button button ) const
		{
		#if WIN32
			return ( m_buttonsPressedThisFrame & static_cast<unsigned int>( button ) ) != 0;
		#else
			return ( ( m_currentButtons & static_cast<unsigned int>( button ) ) != 0 ) &&
				( ( m_previousButtons & static_cast<unsigned int>( button ) ) == 0 );
		#endif
		}

		[[nodiscard]] bool IsUp( Gamepad::Button button ) const
		{
		#if WIN32
			return ( m_buttonsReleasedThisFrame & static_cast<unsigned int>( button ) ) != 0;
		#else
			return ( ( m_currentButtons & static_cast<unsigned int>( button ) ) == 0 ) &&
				( ( m_previousButtons & static_cast<unsigned int>( button ) ) != 0 );
		#endif
		}

		[[nodiscard]] bool IsPressed( Gamepad::Button button ) const
		{
		#if WIN32
			return ( m_currentState.Gamepad.wButtons & static_cast<unsigned int>( button ) ) != 0;
		#else
			return ( m_currentButtons & static_cast<unsigned int>( button ) ) != 0;
		#endif
		}

		[[nodiscard]] bool IsConnected() const
		{
		#if WIN32
			return m_isConnected;
		#else
			return m_gamepad != nullptr;
		#endif
		}

		bool Open( unsigned int joystickID )
		{
		#if WIN32
			( void )joystickID;
			return false;
		#else
			m_gamepad = SDL_OpenGamepad( static_cast<SDL_JoystickID>( joystickID ) );
			m_joystickID = joystickID;
			return m_gamepad != nullptr;
		#endif
		}

		void Close()
		{
		#ifndef WIN32
			if ( m_gamepad != nullptr )
			{
				SDL_CloseGamepad( m_gamepad );
				m_gamepad = nullptr;
			}
			m_joystickID = 0;
			m_currentButtons = 0;
			m_previousButtons = 0;
		#endif
		}

		[[nodiscard]] bool HasJoystickID( unsigned int joystickID ) const
		{
		#if WIN32
			( void )joystickID;
			return false;
		#else
			return m_joystickID == joystickID;
		#endif
		}

		void RegisterButtonEvent( unsigned char button, bool isDown )
		{
		#ifndef WIN32
			struct ButtonMapping
			{
				SDL_GamepadButton sdlButton;
				Gamepad::Button daeButton;
			};

			static constexpr std::array<ButtonMapping, 14> mapping =
			{ {
				{.sdlButton = SDL_GAMEPAD_BUTTON_SOUTH, .daeButton = Button::A },
				{.sdlButton = SDL_GAMEPAD_BUTTON_EAST, .daeButton = Button::B },
				{.sdlButton = SDL_GAMEPAD_BUTTON_WEST, .daeButton = Button::X },
				{.sdlButton = SDL_GAMEPAD_BUTTON_NORTH, .daeButton = Button::Y },
				{.sdlButton = SDL_GAMEPAD_BUTTON_START, .daeButton = Button::Start },
				{.sdlButton = SDL_GAMEPAD_BUTTON_BACK, .daeButton = Button::Back },
				{.sdlButton = SDL_GAMEPAD_BUTTON_LEFT_STICK, .daeButton = Button::LeftThumb },
				{.sdlButton = SDL_GAMEPAD_BUTTON_RIGHT_STICK, .daeButton = Button::RightThumb },
				{.sdlButton = SDL_GAMEPAD_BUTTON_LEFT_SHOULDER, .daeButton = Button::LeftShoulder },
				{.sdlButton = SDL_GAMEPAD_BUTTON_RIGHT_SHOULDER, .daeButton = Button::RightShoulder },
				{.sdlButton = SDL_GAMEPAD_BUTTON_DPAD_UP, .daeButton = Button::DPadUp },
				{.sdlButton = SDL_GAMEPAD_BUTTON_DPAD_DOWN, .daeButton = Button::DPadDown },
				{.sdlButton = SDL_GAMEPAD_BUTTON_DPAD_LEFT, .daeButton = Button::DPadLeft },
				{.sdlButton = SDL_GAMEPAD_BUTTON_DPAD_RIGHT, .daeButton = Button::DPadRight }
			} };

			const auto sdlButton = static_cast<SDL_GamepadButton>( button );
			for ( const auto& mappingEntry : mapping )
			{
				if ( mappingEntry.sdlButton == sdlButton )
				{
					if ( isDown )
					{
						m_currentButtons |= static_cast<unsigned int>( mappingEntry.daeButton );
					}
					else
					{
						m_currentButtons &= ~static_cast<unsigned int>( mappingEntry.daeButton );
					}
					break;
				}
			}
		#else
			(void) button;
			(void) isDown;
		#endif
		}

	private:
		unsigned int m_controllerIndex;
	#if WIN32
		XINPUT_STATE m_currentState{};
		XINPUT_STATE m_previousState{};
		unsigned int m_buttonsPressedThisFrame{};
		unsigned int m_buttonsReleasedThisFrame{};
		bool m_isConnected{};
	#else
		SDL_Gamepad* m_gamepad{ nullptr };
		unsigned int m_joystickID{};
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

	bool Gamepad::Open( unsigned int joystickID )
	{
		return m_pimpl->Open( joystickID );
	}

	void Gamepad::Close()
	{
		m_pimpl->Close();
	}

	bool Gamepad::HasJoystickID( unsigned int joystickID ) const
	{
		return m_pimpl->HasJoystickID( joystickID );
	}

	void Gamepad::RegisterButtonEvent( unsigned char button, bool isDown )
	{
		m_pimpl->RegisterButtonEvent( button, isDown );
	}
}
