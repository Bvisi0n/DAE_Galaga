#ifndef FPSComponent_H
#define FPSComponent_H

#include "Minigin/Core/Component.h"

namespace dae::core
{
	class GameObject;
}

namespace dae::graphics
{
	class Font;
	class TextComponent;
}

namespace bvi::common
{
	class FPSComponent final : public dae::core::Component
	{
	public:
		explicit FPSComponent( dae::core::GameObject* owner );
		~FPSComponent() override = default;

		FPSComponent( const FPSComponent& ) = delete;
		FPSComponent( FPSComponent&& ) = delete;
		FPSComponent& operator=( const FPSComponent& ) = delete;
		FPSComponent& operator=( FPSComponent&& ) = delete;

		void InitializeLinkage() override;
		void InitializeState() override;

		void Update( const float deltaTime ) override;

	private:
		dae::graphics::TextComponent* m_text{ nullptr };
	};
}
#endif
