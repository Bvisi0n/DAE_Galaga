#include <cassert>
#include <chrono>

#include <Minigin/Core/GameObject.h>

#include <Game/GravityBender/GravityBenderBlueprints.h>
#include <Game/GravityBender/GravityRegistry.h>
#include <Game/GravityBender/SpawnGravityFieldCommand.h>

namespace bvi::gravity_bender
{
	SpawnGravityFieldCommand::SpawnGravityFieldCommand( const dae::core::GameObject* contextPlayer )
		: m_playerContext{ contextPlayer }
	{}

	void SpawnGravityFieldCommand::Execute( const float /*deltaTime*/ )
	{
		if ( m_playerContext == nullptr )
		{
			assert( m_playerContext && "SpawnGravityFieldCommand requires a valid player context." );
			return;
		}

		constexpr const auto& gravityFieldConfig = bvi::gravity_bender::config::Config.gravityField;

		const auto currentTime = std::chrono::steady_clock::now();
		const std::chrono::duration<float> timeSinceLastSpawn = currentTime - m_lastSpawnTime;

		if ( timeSinceLastSpawn.count() >= gravityFieldConfig.cooldownSeconds )
		{
			constexpr float radiusSquared{ gravityFieldConfig.effectRadius * gravityFieldConfig.effectRadius };
			const auto origin = m_playerContext->GetTransform().GetWorldPosition();

			const GravityNode newNode
			{
				.position = origin,
				.strength = gravityFieldConfig.pullForce,
				.radiusSquared = radiusSquared,
				.lifeTimeRemaining = gravityFieldConfig.durationSeconds
			};

			GravityRegistry::AddNode( newNode );

			m_lastSpawnTime = currentTime;
		}
	}

	const void* SpawnGravityFieldCommand::GetTargetContext() const
	{
		return m_playerContext;
	}
}
