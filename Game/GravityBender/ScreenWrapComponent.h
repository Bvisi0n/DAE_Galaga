#ifndef BVI_SCREENWRAPCOMPONENT_H
#define BVI_SCREENWRAPCOMPONENT_H

#include <Minigin/Core/Component.h>

// TODO bvi_gravity_bender - Fetch viewport dimensions instead of receiving them as parameters.

namespace dae::core
{
	class GameObject;
}

namespace bvi::gravity_bender
{
	class ScreenWrapComponent final : public dae::core::Component
	{
	public:
		explicit ScreenWrapComponent( dae::core::GameObject* owner, const float viewportWidth, const float viewportHeight, float spriteOffset = 0.0f );

		~ScreenWrapComponent() override;

		ScreenWrapComponent( const ScreenWrapComponent& ) = delete;
		ScreenWrapComponent( ScreenWrapComponent&& ) = delete;
		ScreenWrapComponent& operator=( const ScreenWrapComponent& ) = delete;
		ScreenWrapComponent& operator=( ScreenWrapComponent&& ) = delete;

		void InitializeLinkage() override;

		void InitializeState() override;

		void Update( const float /*deltaTime*/ ) override;

	private:
		float m_viewportWidth;
		float m_viewportHeight;
		float m_spriteOffset;
	};
}

#endif
