#ifndef DAE_SINGLETON_H
#define DAE_SINGLETON_H

#include <cassert>

namespace dae::utils
{
	template <typename T>
	class Singleton
	{
	public:
		[[nodiscard]] static T& GetInstance()
		{
			static T instance{};
		#ifdef _DEBUG
			assert( !s_isDestroyed && "Accessing Singleton after destruction!" );
		#endif
			return instance;
		}

	private:
		friend T;

		Singleton() = default;

		~Singleton()
		{
		#ifdef _DEBUG
			s_isDestroyed = true;
		#endif
		}

		Singleton( const Singleton& other ) = delete; // NOLINT(modernize-use-equals-delete)
		Singleton( Singleton&& other ) = delete; // NOLINT(modernize-use-equals-delete)
		Singleton& operator=( const Singleton& other ) = delete; // NOLINT(modernize-use-equals-delete)
		Singleton& operator=( Singleton&& other ) = delete; // NOLINT(modernize-use-equals-delete)

	#ifdef _DEBUG
		inline static bool s_isDestroyed{ false };
	#endif
	};
}
#endif
