#ifndef SPAWNGRAVITYFIELDCOMMAND_H
#define SPAWNGRAVITYFIELDCOMMAND_H

#include <cassert>
#include <chrono>

#include "Game/GravityBender/GravityRegistry.h"

#include "Minigin/Core/GameObject.h"
#include "Minigin/Input/ICommand.h"

namespace bvi::gravity_bender
{
	class SpawnGravityFieldCommand final : public dae::input::ICommand
	{
	public:
		explicit SpawnGravityFieldCommand( const dae::core::GameObject* contextPlayer )
			: m_playerContext{ contextPlayer }
		{}

		void Execute( const float /*deltaTime*/ ) override
		{
			if ( !m_playerContext )
			{
				assert( m_playerContext && "SpawnGravityFieldCommand requires a valid player context." );
				return;
			}

			auto currentTime = std::chrono::steady_clock::now();
			std::chrono::duration<float> timeSinceLastSpawn = currentTime - m_lastSpawnTime;

			constexpr float spawnCooldown = 1.0f;
			if ( timeSinceLastSpawn.count() >= spawnCooldown )
			{
				const auto origin = m_playerContext->GetTransform().GetWorldPosition();

				constexpr float radius = 100.0f;
				constexpr float strength = 1'500'000.0f;
				GravityRegistry::AddNode( { origin, strength, radius * radius } );

				m_lastSpawnTime = currentTime;
			}
		}

		[[nodiscard]] const void* GetTargetContext() const override
		{
			return m_playerContext;
		}

	private:
		const dae::core::GameObject* m_playerContext;
		std::chrono::steady_clock::time_point m_lastSpawnTime{};
	};
}
#endif
