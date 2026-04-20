#ifndef UIVALUEOBSERVER_H
#define UIVALUEOBSERVER_H

#include <functional>
#include <string>

#include "Minigin/Core/Component.h"
#include "Minigin/Events/GameEvent.h"
#include "Minigin/Events/IObserver.h"

namespace dae::core
{
	class GameObject;
}

namespace dae::graphics
{
	class TextComponent;
}

namespace bvi::galaga
{
	class UIValueObserver final : public dae::core::Component, public dae::events::IObserver
	{
	public:
		using Formatter = std::function<std::string( int )>;

		UIValueObserver( dae::core::GameObject* owner, dae::events::GameEvent observedEvent, Formatter formatter );

		void InitializeLinkage() override;
		void InitializeState() override;

		void Update( const float deltaTime ) override;

		void OnNotify( const dae::events::GameEvent event ) override;

	private:
		Formatter m_formatter;
		dae::graphics::TextComponent* m_text{ nullptr };
		dae::events::GameEvent m_observedEvent; // TODO GALA: Does it have to be a whole GameEvent object?
	};
}
#endif
