#include <cassert>
#include <memory>
#include <utility>
#include <vector>

#include <glm/ext/matrix_float4x4.hpp>

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
		const size_t object_count = m_pObjects.size();
		for ( size_t i = 0; i < object_count; ++i )
		{
			if ( !m_pObjects[ i ]->IsPendingDeletion() )
			{
				m_pObjects[ i ]->Update( deltaTime );
			}
		}

		for ( auto& p_object : m_pObjects )
		{
			if ( !p_object->IsPendingDeletion() && !p_object->GetParent() )
			{
				p_object->GetTransform().UpdateWorldMatrix( glm::mat4( 1.0f ) );
			}
		}

		CleanupGameObjects();
	}

	void Scene::Render() const
	{
		for ( const auto& p_object : m_pObjects )
		{
			p_object->Render();
		}
	}

	void Scene::AddGameObject( std::unique_ptr<core::GameObject> pObject )
	{
		assert( pObject != nullptr && "Cannot add a null GameObject to the scene." );

		m_pPendingObjects.emplace_back( std::move( pObject ) );
	}

	void Scene::RemoveGameObject( core::GameObject& object )
	{
		object.MarkForDeletion();
	}

	void Scene::CleanupGameObjects()
	{
		std::erase_if( m_pObjects,
			[] ( const auto& pObj )
			{
				return pObj->IsPendingDeletion();
			} );
	}

	void Scene::RemoveAllGameObjects()
	{
		m_pObjects.clear();
	}

	void Scene::FlushPendingObjects()
	{
		if ( m_pPendingObjects.empty() )
		{
			return;
		}

		// Allows adding new objects during the flush without iterator invalidation issues, they will be flushed in the next frame.
		auto p_new_objects = std::move( m_pPendingObjects );
		m_pPendingObjects.clear();

		const size_t new_index_start = m_pObjects.size();

		m_pObjects.reserve( m_pObjects.size() + p_new_objects.size() );
		for ( auto& p_object : p_new_objects )
		{
			m_pObjects.emplace_back( std::move( p_object ) );
		}

		constexpr size_t required_initialization_passes = 2;

		for ( size_t phase = 0; phase < required_initialization_passes; ++phase )
		{
			const size_t object_count = m_pObjects.size();
			for ( size_t index = new_index_start; index < object_count; ++index )
			{
				// 2 States: Link -> Ready (set initial values)
				m_pObjects[ index ]->AdvanceComponentStates();
			}
		}
	}
}
