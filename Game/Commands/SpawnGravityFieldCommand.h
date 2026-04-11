#ifndef SPAWNGRAVITYFIELDCOMMAND_H
#define SPAWNGRAVITYFIELDCOMMAND_H

#include "Game/Physics/GravityRegistry.h"

#include "Minigin/Core/GameObject.h"
#include "Minigin/Input/Command.h"
#include "Minigin/Scene/SceneManager.h"

namespace bvi::commands
{
	class SpawnGravityFieldCommand final : public dae::input::Command
	{
	public:
		explicit SpawnGravityFieldCommand( const dae::core::GameObject* contextPlayer )
			: m_playerContext{ contextPlayer }
		{}

		void Execute( const float /*deltaTime*/ ) override
		{
			const auto origin = m_playerContext->GetTransform().GetWorldPosition();

			constexpr float radius = 1000.0f;
			constexpr float strength = 20000.0f;
			physics::GravityRegistry::AddNode( { origin, strength, radius * radius } );
		}

		[[nodiscard]] const void* GetTargetContext() const override
		{
			return m_playerContext;
		}

	private:
		const dae::core::GameObject* m_playerContext;
	};
}
#endif
