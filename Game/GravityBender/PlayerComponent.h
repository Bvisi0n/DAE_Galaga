#ifndef PLAYERGRAVITYCOMPONENT_H
#define PLAYERGRAVITYCOMPONENT_H

#include "Minigin/Core/Component.h"
#include "Minigin/Core/GameObject.h"

namespace bvi::gravity_bender
{
	class PlayerComponent final : public dae::core::Component
	{
	public:
		explicit PlayerComponent( dae::core::GameObject* owner );

		~PlayerComponent() override;

		void InitializeLinkage() override;
		void InitializeState() override;

		void Update( const float /*deltaTime*/ ) override;

	};
}
#endif