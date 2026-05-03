#ifndef BVI_SCREENWRAPCOMPONENT_H
#define BVI_SCREENWRAPCOMPONENT_H

#include <Minigin/Core/Component.h>

namespace dae::core
{
	class GameObject;
}

namespace bvi::gravity_bender
{
	class ScreenWrapComponent final : public dae::core::Component
	{
	public:
		explicit ScreenWrapComponent( dae::core::GameObject* owner, float spriteOffset );

		~ScreenWrapComponent() override;

		ScreenWrapComponent( const ScreenWrapComponent& ) = delete;
		ScreenWrapComponent( ScreenWrapComponent&& ) = delete;
		ScreenWrapComponent& operator=( const ScreenWrapComponent& ) = delete;
		ScreenWrapComponent& operator=( ScreenWrapComponent&& ) = delete;

		void InitializeLinkage() override;

		void InitializeState() override;

		void Update( float deltaTime ) override;

	private:
		float m_spriteOffset;
	};
}

#endif
