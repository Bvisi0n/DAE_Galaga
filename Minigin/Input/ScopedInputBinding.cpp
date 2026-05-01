#include <memory>
#include <type_traits>
#include <utility>
#include <variant>

#include <Minigin/Input/Gamepad.h>
#include <Minigin/Input/ICommand.h>
#include <Minigin/Input/InputManager.h>
#include <Minigin/Input/Keyboard.h>
#include <Minigin/Input/ScopedInputBinding.h>

namespace dae::input
{
	ScopedInputBinding::ScopedInputBinding( Keyboard::Key key, InputManager::KeyState state, std::unique_ptr<ICommand> command )
		: m_track{ KeyboardTrack{.key = key, .state = state} }
	{
		InputManager::GetInstance().BindCommand( key, state, std::move( command ) );
	}

	ScopedInputBinding::ScopedInputBinding( Gamepad::Button button, InputManager::KeyState state, unsigned int controllerIndex, std::unique_ptr<ICommand> command )
		: m_track{ GamepadTrack{.controllerIndex = controllerIndex, .button = button, .state = state} }
	{
		InputManager::GetInstance().BindCommand( button, state, std::move( command ), controllerIndex );
	}

	ScopedInputBinding::~ScopedInputBinding()
	{
		Unbind();
	}

	ScopedInputBinding::ScopedInputBinding( ScopedInputBinding&& other ) noexcept
		: m_track{ std::exchange( other.m_track, std::monostate{} ) }
	{}

	ScopedInputBinding& ScopedInputBinding::operator=( ScopedInputBinding&& other ) noexcept
	{
		if ( this != &other )
		{
			Unbind();
			m_track = std::exchange( other.m_track, std::monostate{} );
		}
		return *this;
	}

	void ScopedInputBinding::Unbind()
	{
		std::visit( [] ( auto&& argument )
		{
			using T = std::decay_t<decltype( argument )>;

			if constexpr ( std::is_same_v<T, KeyboardTrack> )
			{
				InputManager::GetInstance().UnbindCommand( argument.key, argument.state );
			}
			else if constexpr ( std::is_same_v<T, GamepadTrack> )
			{
				InputManager::GetInstance().UnbindCommand( argument.button, argument.state, argument.controllerIndex );
			}
			// If std::monostate, do nothing

		}, m_track );
	}
}
