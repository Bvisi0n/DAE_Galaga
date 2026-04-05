#ifndef UIVALUEOBSERVER_H
#define UIVALUEOBSERVER_H

#include <functional>
#include <string>
#include <vector>

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

namespace bvi
{
	class UIValueObserver final : public dae::core::Component, public dae::events::IObserver
	{
	public:
		using Formatter = std::function<std::string(int)>;

		UIValueObserver(dae::core::GameObject* pOwner, dae::events::GameEvent observedEvent, Formatter formatter);

		void Initialize() override;

		void OnNotify(const dae::events::GameEvent event) override;

		void Update(const float) override {}

	private:
		Formatter m_Formatter;
		dae::graphics::TextComponent* m_pText{ nullptr };
		dae::events::GameEvent m_observedEvent; // TODO L: Does it have to be a whole GameEvent object?
	};
}
#endif
