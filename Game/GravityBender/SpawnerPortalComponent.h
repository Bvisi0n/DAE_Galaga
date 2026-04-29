#ifndef BVI_PORTALSPAWNERCOMPONENT_H
#define BVI_PORTALSPAWNERCOMPONENT_H

#include <glm/ext/vector_float3.hpp>

#include "Minigin/Core/Component.h"

namespace dae::core
{
	class GameObject;
}

namespace dae::graphics
{
	class PrimitiveRenderComponent;
}

namespace bvi::gravity_bender
{
	class SpawnerPortalComponent final : public dae::core::Component
	{
	public:
		explicit SpawnerPortalComponent( dae::core::GameObject* owner, const UnitData& blueprint );
		~SpawnerPortalComponent() override = default;

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

		UnitData m_blueprint;
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
