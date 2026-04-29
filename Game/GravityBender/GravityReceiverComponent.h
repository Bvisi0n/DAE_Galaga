#ifndef BVI_GRAVITYRECEIVERCOMPONENT_H
#define BVI_GRAVITYRECEIVERCOMPONENT_H

#include <Minigin/Core/Component.h>

namespace dae::core
{
	class GameObject;
	class MoveComponent;
}

namespace bvi::gravity_bender
{
	class GravityReceiverComponent final : public dae::core::Component
	{
	public:
		explicit GravityReceiverComponent( dae::core::GameObject* owner );
		~GravityReceiverComponent() override;

		GravityReceiverComponent( const GravityReceiverComponent& ) = delete;
		GravityReceiverComponent( GravityReceiverComponent&& ) = delete;
		GravityReceiverComponent& operator=( const GravityReceiverComponent& ) = delete;
		GravityReceiverComponent& operator=( GravityReceiverComponent&& ) = delete;

		void InitializeLinkage() override;
		void InitializeState() override;

		void Update( const float ) override;

	private:
		dae::core::MoveComponent* m_moveComponent{ nullptr };
	};
}
#endif
