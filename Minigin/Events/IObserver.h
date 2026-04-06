#ifndef IOBSERVER_H
#define IOBSERVER_H

#include "Minigin/Events/GameEvent.h"

namespace dae::events
{
	class IObserver
	{
		// https://gameprogrammingpatterns.com/observer.html
		// https://refactoring.guru/design-patterns/observer
		// https://refactoring.guru/design-patterns/observer/cpp/example
	public:
		virtual ~IObserver()
		{};

		virtual void OnNotify( const GameEvent event ) = 0;
	};
}
#endif
