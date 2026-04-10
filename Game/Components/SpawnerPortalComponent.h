#ifndef PORTALSPAWNERCOMPONENT_H
#define PORTALSPAWNERCOMPONENT_H

#include "Game/Blueprints/GravityBenderBlueprints.h"

#include "Minigin/Core/Component.h"
#include "Minigin/Core/GameObject.h"

namespace bvi::components
{
	class SpawnerPortalComponent final : public dae::core::Component
	{
	public:
		SpawnerPortalComponent( dae::core::GameObject* owner, const blueprints::ZakoData& blueprint );
		virtual ~SpawnerPortalComponent() = default;

		SpawnerPortalComponent( const SpawnerPortalComponent& ) = delete;
		SpawnerPortalComponent( SpawnerPortalComponent&& ) = delete;
		SpawnerPortalComponent& operator=( const SpawnerPortalComponent& ) = delete;
		SpawnerPortalComponent& operator=( SpawnerPortalComponent&& ) = delete;

		void InitializeLinkage() override;
		void InitializeState() override;

		void Update( const float deltaTime ) override;

	private:
		enum class PortalState
		{
			Anticipation, Spawning, Exhausted
		};

		PortalState m_currentState{ PortalState::Anticipation };
		blueprints::ZakoData m_blueprint;

		float m_timer{ 0.0f };
		int m_spawnedCount{ 0 };
		const float m_anticipationDuration{ 3.0f };

		void EmitZako();
	};
}
#endif
