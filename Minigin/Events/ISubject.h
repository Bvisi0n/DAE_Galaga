#ifndef ISUBJECT_H
#define ISUBJECT_H

namespace dae::events
{
	class IObserver;
	struct GameEvent;

	class ISubject
	{
		// https://gameprogrammingpatterns.com/observer.html
		// https://refactoring.guru/design-patterns/observer
		// https://refactoring.guru/design-patterns/observer/cpp/example
	public:
		virtual ~ISubject() = default;
		
		ISubject(const ISubject&)               = delete;
		ISubject& operator=(const ISubject&)    = delete;
		ISubject(ISubject&&)                    = delete;
		ISubject& operator=(ISubject&&)         = delete;
		
		virtual void AttachObserver(IObserver* observer) = 0;
		virtual void DetachObserver(IObserver* observer) = 0;

	protected:
		ISubject() = default;
		virtual void NotifyObservers(GameEvent event) const = 0;
	};
}
#endif
