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
		#ifdef _DEBUG
			assert( !m_isDestroyed && "Accessing Singleton after destruction!" );
		#endif
			return instance;
		}

	protected:
		Singleton() = default;
		virtual ~Singleton()
		{
		#ifdef _DEBUG
			m_isDestroyed = true;
		#endif
		}

	private:
	#ifdef _DEBUG
		inline static bool m_isDestroyed{ false };
	#endif
	};
}
#endif
