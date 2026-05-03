#ifndef DAE_KEYBOARD_H
#define DAE_KEYBOARD_H

#include <memory>

namespace dae::input
{

	class Keyboard final
	{
	public:
		enum class Key : std::uint8_t
		{
			W, A, S, D,
			Space, Q, E, F, G
		};

		Keyboard();
		~Keyboard();

		Keyboard( const Keyboard& ) = delete;
		Keyboard& operator=( const Keyboard& ) = delete;

		Keyboard( Keyboard&& ) noexcept;
		Keyboard& operator=( Keyboard&& ) noexcept;

		void Update();
		[[nodiscard]] bool IsDown( Key key ) const;
		[[nodiscard]] bool IsUp( Key key ) const;
		[[nodiscard]] bool IsPressed( Key key ) const;

	private:
		class KeyboardImpl;
		std::unique_ptr<KeyboardImpl> m_pimpl;
	};
}

#endif
