#include <memory>
#include <unordered_map>
#include <vector>

#if WIN32
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#else
#include <SDL3/SDL.h>
#endif

#include "Minigin/Input/Keyboard.h"

namespace dae::input
{
	class Keyboard::KeyboardImpl
	{
	public:
		KeyboardImpl() = default;
		~KeyboardImpl() = default;

		void Update()
		{
			m_previousState = m_currentState;

		#if WIN32
			BYTE state[ 256 ];
			if ( GetKeyboardState( state ) )
			{
				m_currentState.assign( state, state + 256 );
			}
		#else
			int numKeys{};
			const bool* state = SDL_GetKeyboardState( &numKeys );
			m_currentState.assign( state, state + numKeys );
		#endif

			if ( m_previousState.empty() )
			{
				m_previousState = m_currentState;
			}
		}

		bool IsPressed( const Key key ) const
		{
			int platformKey = GetPlatformKey( key );
		#if WIN32
			return m_currentState[ platformKey ] & 0x80;
		#else
			return m_currentState[ platformKey ];
		#endif
		}

		bool IsDown( const Key key ) const
		{
			return IsPressed( key ) && !WasPressed( key );
		}

		bool IsUp( const Key key ) const
		{
			return !IsPressed( key ) && WasPressed( key );
		}

	private:
		bool WasPressed( const Key key ) const
		{
			int platformKey = GetPlatformKey( key );
		#if WIN32
			return m_previousState[ platformKey ] & 0x80;
		#else
			return m_previousState[ platformKey ];
		#endif
		}

		int GetPlatformKey( const Key key ) const
		{
		#if WIN32
			// https://learn.microsoft.com/en-us/windows/win32/inputdev/virtual-key-codes
			static const std::unordered_map<Key, int> keyMap =
			{
				{Key::W,        'W'},
				{Key::A,        'A'},
				{Key::S,        'S'},
				{Key::D,        'D'},
				{Key::Space,    VK_SPACE},
				{Key::Q,        'Q'},
				{Key::E,        'E'},
				{Key::F,        'F'},
			};
		#else
			// https://wiki.libsdl.org/SDL3/SDL_Scancode
			static const std::unordered_map<Key, int> keyMap =
			{
				{Key::W,        SDL_SCANCODE_W},
				{Key::A,        SDL_SCANCODE_A},
				{Key::S,        SDL_SCANCODE_S},
				{Key::D,        SDL_SCANCODE_D},
				{Key::Space,    SDL_SCANCODE_SPACE},
				{Key::Q,        SDL_SCANCODE_Q},
				{Key::E,        SDL_SCANCODE_E},
				{Key::F,        SDL_SCANCODE_F},
			};
		#endif
			return keyMap.at( key );
		}

		std::vector<unsigned char> m_currentState;
		std::vector<unsigned char> m_previousState;
	};

	Keyboard::Keyboard()
		: m_pimpl( std::make_unique<KeyboardImpl>() )
	{}

	// Don't remove, needs to be here for definition of KeyboardImpl.
	Keyboard::~Keyboard() = default;

	void Keyboard::Update()
	{
		m_pimpl->Update();
	}

	bool Keyboard::IsDown( const Key key ) const
	{
		return m_pimpl->IsDown( key );
	}

	bool Keyboard::IsUp( const Key key ) const
	{
		return m_pimpl->IsUp( key );
	}

	bool Keyboard::IsPressed( const Key key ) const
	{
		return m_pimpl->IsPressed( key );
	}
}
