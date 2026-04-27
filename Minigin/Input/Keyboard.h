#ifndef DAE_KEYBOARD_H
#define DAE_KEYBOARD_H

#include <memory>

namespace dae::input
{

	class Keyboard final
	{
	public:
		enum class Key
		{
			W, A, S, D,
			Space, Q, E, F, G
		};

		Keyboard();
		~Keyboard();

		void Update();
		[[nodiscard]] bool IsDown( const Key key ) const;
		[[nodiscard]] bool IsUp( const Key key ) const;
		[[nodiscard]] bool IsPressed( const Key key ) const;

	private:
		class KeyboardImpl;
		std::unique_ptr<KeyboardImpl> m_pimpl;
	};
}

#endif
