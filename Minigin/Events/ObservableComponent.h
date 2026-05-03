#ifndef DAE_OBSERVABLECOMPONENT_H
#define DAE_OBSERVABLECOMPONENT_H

#include <vector>

#include <Minigin/Core/Component.h>
#include <Minigin/Events/ISubject.h>

namespace dae::core
{
	class GameObject;
}

namespace dae::events
{
	class IObserver;
	struct GameEvent;

	class ObservableComponent : public core::Component, public ISubject
	{
	public:
		explicit ObservableComponent( core::GameObject* owner ) noexcept
			: Component( owner )
		{};
		~ObservableComponent() override = default;

		ObservableComponent( const ObservableComponent& ) = delete;
		ObservableComponent( ObservableComponent&& ) = delete;
		ObservableComponent& operator=( const ObservableComponent& ) = delete;
		ObservableComponent& operator=( ObservableComponent&& ) = delete;

		void InitializeLinkage() override = 0;
		void InitializeState() override = 0;
		void Update( float deltaTime ) override = 0;

		void AttachObserver( IObserver* observer ) override;
		void DetachObserver( IObserver* observer ) override;

	protected:
		void NotifyObservers( GameEvent event ) override;

	private:
		std::vector<IObserver*> m_observers;
		std::vector<IObserver*> m_pendingObservers;
		mutable bool m_isNotifying{ false };
	};
}
#endif
