#include <cassert>
#include <chrono>

#include <Minigin/Core/GameObject.h>

#include <Game/GravityBender/GravityRegistry.h>
#include <Game/GravityBender/SpawnGravityFieldCommand.h>

namespace bvi::gravity_bender
{
	SpawnGravityFieldCommand::SpawnGravityFieldCommand( const dae::core::GameObject* contextPlayer )
		: m_playerContext{ contextPlayer }
	{}

	void SpawnGravityFieldCommand::Execute( const float )
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

	const void* SpawnGravityFieldCommand::GetTargetContext() const
	{
		return m_playerContext;
	}
}
