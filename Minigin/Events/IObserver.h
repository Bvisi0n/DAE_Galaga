#ifndef DAE_IOBSERVER_H
#define DAE_IOBSERVER_H

#include "Minigin/Events/GameEvent.h"

namespace dae::events
{
	class IObserver
	{
		// https://gameprogrammingpatterns.com/observer.html
		// https://refactoring.guru/design-patterns/observer
		// https://refactoring.guru/design-patterns/observer/cpp/example
	public:
		IObserver() = default;
		virtual ~IObserver() = default;

		IObserver( const IObserver& ) = delete;
		IObserver( IObserver&& ) = delete;
		IObserver& operator=( const IObserver& ) = delete;
		IObserver& operator=( IObserver&& ) = delete;

		virtual void OnNotify( const GameEvent event ) = 0;
	};
}
#endif
