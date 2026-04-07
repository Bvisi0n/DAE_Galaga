#include <cassert>
#include <memory>
#include <utility>
#include <vector>

#include <glm/ext/matrix_float4x4.hpp>

#include <Minigin/Core/CollisionSystem.h>
#include <Minigin/Core/GameObject.h>
#include "Minigin/Core/Transform.h"
#include "Minigin/Scene/Scene.h"

namespace dae::scene
{
	void Scene::Initialize()
	{
		if ( m_isInitialized )
		{
			assert( !m_isInitialized && "Scene is already initialized." );
			return;
		}

		FlushPendingObjects();

		m_isInitialized = true;
	}

	void Scene::Update( const float deltaTime )
	{
		assert( m_isInitialized && "Scene isn't initialized." );
		FlushPendingObjects();

		// std::vector iterator invalidation
		const size_t objectCount = m_objects.size();
		for ( size_t i = 0; i < objectCount; ++i )
		{
			if ( !m_objects[ i ]->IsPendingDeletion() )
			{
				m_objects[ i ]->Update( deltaTime );
			}
		}

		for ( auto& object : m_objects )
		{
			if ( !object->IsPendingDeletion() && !object->GetParent() )
			{
				object->GetTransform().UpdateWorldMatrix( glm::mat4( 1.0f ) );
			}
		}

		m_collisionSystem.Update( m_objects );

		CleanupGameObjects();
	}

	void Scene::Render() const
	{
		for ( const auto& object : m_objects )
		{
			object->Render();
		}
	}

	void Scene::AddGameObject( std::unique_ptr<core::GameObject> object )
	{
		assert( object != nullptr && "Cannot add a null GameObject to the scene." );

		m_pendingObjects.emplace_back( std::move( object ) );
	}

	void Scene::RemoveGameObject( core::GameObject& object )
	{
		object.MarkForDeletion();
	}

	void Scene::CleanupGameObjects()
	{
		std::erase_if( m_objects,
			[] ( const auto& object )
			{
				return object->IsPendingDeletion();
			} );
	}

	void Scene::RemoveAllGameObjects()
	{
		m_objects.clear();
	}

	core::CollisionSystem& Scene::GetCollisionSystem()
	{
		return m_collisionSystem;
	}

	void Scene::FlushPendingObjects()
	{
		if ( m_pendingObjects.empty() )
		{
			return;
		}

		// Allows adding new objects during the flush without iterator invalidation issues, they will be flushed in the next frame.
		auto newObjects = std::move( m_pendingObjects );
		m_pendingObjects.clear();

		const size_t newIndexStart = m_objects.size();

		m_objects.reserve( m_objects.size() + newObjects.size() );
		for ( auto& object : newObjects )
		{
			m_objects.emplace_back( std::move( object ) );
		}

		constexpr size_t requiredInitPasses = 2;

		for ( size_t phase = 0; phase < requiredInitPasses; ++phase )
		{
			const size_t objectCount = m_objects.size();
			for ( size_t index = newIndexStart; index < objectCount; ++index )
			{
				// 2 States: Link -> Ready (set initial values)
				m_objects[ index ]->AdvanceComponentStates();
			}
		}
	}
}
