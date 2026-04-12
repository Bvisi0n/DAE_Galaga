#ifndef PORTALSPAWNERCOMPONENT_H
#define PORTALSPAWNERCOMPONENT_H

#include <glm/vec3.hpp>

#include "Game/Blueprints/GravityBenderBlueprints.h"

#include "Minigin/Core/Component.h"
#include "Minigin/Core/GameObject.h"

namespace dae::graphics
{
	class PrimitiveRenderComponent;
}

namespace bvi::components
{
	class SpawnerPortalComponent final : public dae::core::Component
	{
	public:
		SpawnerPortalComponent( dae::core::GameObject* owner, const blueprints::UnitData& blueprint );
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

		blueprints::UnitData m_blueprint;
		dae::graphics::PrimitiveRenderComponent* m_primitiveRenderer{ nullptr };

		glm::vec3 m_direction{};
		PortalState m_currentState{ PortalState::Anticipation };
		float m_timer{ 0.0f };
		int m_spawnedCount{ 0 };
		const float m_CooldownDuration{ 15.0f };
		const float m_anticipationDuration{ 3.0f };

		void EmitUnit();
		void SetRandomDirection();
		void SetRandomPosition();

	};
}
#endif
