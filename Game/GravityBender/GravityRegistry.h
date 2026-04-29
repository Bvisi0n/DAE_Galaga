#ifndef BVI_GRAVITYREGISTRY_H
#define BVI_GRAVITYREGISTRY_H

#include <vector>

#include <glm/ext/vector_float3.hpp>

namespace dae::core
{
	class GameObject;
}

namespace bvi::gravity_bender
{
	struct GravityNode
	{
		glm::vec3 position;
		float strength;
		float radiusSquared;
		float lifeTimeRemaining{ 3.0f };
		dae::core::GameObject* visualRoot{ nullptr };
		bool isPlayer{ false };
	};

	class GravityRegistry final
	{
	public:
		GravityRegistry() = delete;

		static void Update( float deltaTime );

		static void AddNode( const GravityNode& node );
		static void SetPlayerNode( const GravityNode& node );
		static void RemovePlayerNode();
		static void ClearNodes();

		[[nodiscard]] static const std::vector<GravityNode>& GetActiveNodes() noexcept;

	private:
		static inline std::vector<GravityNode> s_nodes{};
	};
}
#endif
