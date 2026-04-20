#ifndef PLAYERGRAVITYCOMPONENT_H
#define PLAYERGRAVITYCOMPONENT_H

#include "Game/GravityBender/GravityRegistry.h"

#include "Minigin/Core/Component.h"
#include "Minigin/Core/GameObject.h"

namespace bvi::gravity_bender
{
	class PlayerGravityComponent final : public dae::core::Component
	{
	public:
		explicit PlayerGravityComponent( dae::core::GameObject* owner )
			: dae::core::Component( owner )
		{}

		~PlayerGravityComponent() override
		{
			GravityRegistry::RemovePlayerNode();
		}

		void InitializeLinkage() override
		{}

		void InitializeState() override
		{}

		void Update( const float /*deltaTime*/ ) override
		{
			// TODO GRAV: For prototyping purposes, there are better ways to do this.
			const auto& origin = GetOwner()->GetTransform().GetWorldPosition();
			constexpr float radius = 100.0f;
			constexpr float strength = 1'500'000.0f;
			GravityRegistry::SetPlayerNode( GravityNode{ origin, strength, radius * radius, std::numeric_limits<float>::infinity() } );
		}
	};
}
#endif