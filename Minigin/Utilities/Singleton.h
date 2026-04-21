#ifndef SINGLETON_H
#define SINGLETON_H

#include <cassert>

namespace dae::utils
{
	template <typename T>
	class Singleton
	{
	public:

		Singleton( const Singleton& other ) = delete;
		Singleton( Singleton&& other ) = delete;
		Singleton& operator=( const Singleton& other ) = delete;
		Singleton& operator=( Singleton&& other ) = delete;

		[[nodiscard]] static T& GetInstance()
		{
			static T instance{};
			assert( !m_isDestroyed && "Accessing Singleton after destruction!" );
			return instance;
		}

	protected:
		Singleton() = default;
		virtual ~Singleton()
		{
			m_isDestroyed = true;
		}

	private:
		inline static bool m_isDestroyed{ false };
	};
}
#endif
