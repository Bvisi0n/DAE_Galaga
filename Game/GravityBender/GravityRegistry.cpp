#include <algorithm>
#include <array>
#include <cmath>
#include <limits>
#include <memory>
#include <utility>
#include <vector>

#include <glm/ext/vector_float3.hpp>

#include <SDL3/SDL_pixels.h>

#include <Minigin/Core/GameObject.h>
#include <Minigin/Graphics/PrimitiveRenderComponent.h>
#include <Minigin/Scene/Scene.h>
#include <Minigin/Scene/SceneManager.h>

#include <Game/GravityBender/GravityBenderBlueprints.h>
#include <Game/GravityBender/GravityRegistry.h>

namespace bvi::gravity_bender
{
	void GravityRegistry::Update( float deltaTime )
	{
		for ( auto& node : s_nodes )
		{
			if ( node.type != GravityNodeType::Player )
			{
				node.lifeTimeRemaining -= deltaTime;
			}

			if ( node.lifeTimeRemaining <= 0.0F )
			{
				if ( node.visualRoot != nullptr )
				{
					dae::scenes::Scene::RemoveGameObject( *node.visualRoot );
				}

				node.visualRoot = nullptr;
			}
		}

		std::erase_if
		(
			s_nodes,
			[] ( const GravityNode& node )
			{
				return node.lifeTimeRemaining <= 0.0F;
			}
		);
	}

	void GravityRegistry::AddNode( const GravityNode& node )
	{
		s_nodes.push_back( node );
		auto& storedNode = s_nodes.back();

		using PrimitiveRenderComponent = dae::graphics::PrimitiveRenderComponent;
		using PrimitiveShape = dae::graphics::PrimitiveShape;
		using CircleShape = dae::graphics::CircleShape;

		const float radius = std::sqrt( node.radiusSquared );
		float coreRadius = 0.F;

		const auto& blueprintColor = config::Config.gravityField.color;

		switch ( node.type )
		{
			case GravityNodeType::Player:
				coreRadius = config::Config.player.colliderSize;
				break;
			case GravityNodeType::Field:
			default:
				coreRadius = config::Config.gravityField.coreSize;
				break;
		}

		const SDL_Color coreColor{ blueprintColor.r, blueprintColor.g, blueprintColor.b, blueprintColor.a };
		const SDL_Color rangeColor{ blueprintColor.r, blueprintColor.g, blueprintColor.b, 128 };

		auto centerVisualRoot = std::make_unique<dae::core::GameObject>();
		centerVisualRoot->GetTransform().SetLocalPosition( node.position );
		centerVisualRoot->AddComponent<PrimitiveRenderComponent>( PrimitiveShape{ CircleShape{ coreRadius, true } }, coreColor );

		auto centerVisualOuter = std::make_unique<dae::core::GameObject>();
		centerVisualOuter->AddComponent<PrimitiveRenderComponent>( PrimitiveShape{ CircleShape{ radius, false } }, rangeColor );
		centerVisualOuter->SetParent( centerVisualRoot.get(), false );

		storedNode.visualRoot = centerVisualRoot.get();
		auto* pCenterRoot = centerVisualRoot.get();
		auto& scene = dae::scenes::SceneManager::GetInstance().GetActiveScene();

		scene.AddGameObject( std::move( centerVisualRoot ) );
		scene.AddGameObject( std::move( centerVisualOuter ) );

		const float width = config::Config.viewport.width;
		const float height = config::Config.viewport.height;
		const std::array<glm::vec3, 8> offsets = { {
			{0.F, -height, 0.F}, {width, 0.F, 0.F}, {0.F, height, 0.F}, {-width, 0.F, 0.F},
			{width, -height, 0.F}, {width, height, 0.F}, {-width, height, 0.F}, {-width, -height, 0.F}
		} };

		for ( const auto& offset : offsets )
		{
			auto cloneRoot = std::make_unique<dae::core::GameObject>();
			cloneRoot->GetTransform().SetLocalPosition( offset );
			cloneRoot->AddComponent<PrimitiveRenderComponent>( PrimitiveShape{ CircleShape{ coreRadius, true } }, coreColor );

			auto cloneOuter = std::make_unique<dae::core::GameObject>();
			cloneOuter->AddComponent<PrimitiveRenderComponent>( PrimitiveShape{ CircleShape{ radius, false } }, rangeColor );

			cloneOuter->SetParent( cloneRoot.get(), false );
			cloneRoot->SetParent( pCenterRoot, false );

			scene.AddGameObject( std::move( cloneRoot ) );
			scene.AddGameObject( std::move( cloneOuter ) );
		}
	}

	void GravityRegistry::SetPlayerNode( const GravityNode& node )
	{
		auto iterator = std::ranges::find_if
		(
			s_nodes,
			[] ( const GravityNode& n )
			{
				return n.type == GravityNodeType::Player;
			}
		);

		if ( iterator == s_nodes.end() )
		{
			GravityNode playerNode = node;
			playerNode.lifeTimeRemaining = std::numeric_limits<float>::infinity();
			playerNode.type = GravityNodeType::Player;

			AddNode( playerNode );
		}
		else
		{
			iterator->position = node.position;
			iterator->strength = node.strength;
			iterator->radiusSquared = node.radiusSquared;

			if ( iterator->visualRoot != nullptr )
			{
				iterator->visualRoot->GetTransform().SetLocalPosition( node.position );
			}
		}
	}

	void GravityRegistry::RemovePlayerNode()
	{
		auto iterator = std::ranges::find_if
		(
			s_nodes,
			[] ( const GravityNode& n )
			{
				return n.type == GravityNodeType::Player;
			}
		);

		if ( iterator != s_nodes.end() )
		{
			if ( iterator->visualRoot != nullptr )
			{
				dae::scenes::Scene::RemoveGameObject( *( iterator->visualRoot ) );
			}

			s_nodes.erase( iterator );
		}
	}

	void GravityRegistry::ClearNodes()
	{
		for ( const auto& node : s_nodes )
		{
			if ( node.visualRoot != nullptr )
			{
				dae::scenes::Scene::RemoveGameObject( *node.visualRoot );
			}
		}

		s_nodes.clear();
	}

	const std::vector<GravityNode>& GravityRegistry::GetActiveNodes() noexcept
	{
		return s_nodes;
	}
}
