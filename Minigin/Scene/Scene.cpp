#include <algorithm>
#include <cassert>

#include "Minigin/Core/Transform.h"
#include "Minigin/Scene/Scene.h"

namespace dae::scene
{
	void Scene::Initialize()
	{
		if (m_isInitialized)
		{
			assert(!m_isInitialized && "Scene is already initialized.");
			return;
		}

		FlushPendingObjects();

		m_isInitialized = true;
	}

	void Scene::Update(const float deltaTime)
	{
		assert(m_isInitialized && "Scene isn't initialized.");
		FlushPendingObjects();

		// Don't change to ranged based for loop:
		// std::vector iterator invalidation
		const size_t objectCount = m_pObjects.size();
		for (size_t i = 0; i < objectCount; ++i)
		{
			if (!m_pObjects[i]->IsPendingDeletion())
			{
				m_pObjects[i]->Update(deltaTime);
			}
		}

		for (auto& object : m_pObjects)
		{
			if (!object->IsPendingDeletion() && !object->GetParent())
			{
				object->GetTransform().UpdateWorldMatrix(glm::mat4(1.0f));
			}
		}

		CleanupGameObjects();
	}

	void Scene::Render() const
	{
		for (const auto& object : m_pObjects)
		{
			object->Render();
		}
	}

	void Scene::AddGameObject(std::unique_ptr<core::GameObject> pObject)
	{
		assert(pObject != nullptr && "Cannot add a null GameObject to the scene.");

		m_pPendingObjects.emplace_back(std::move(pObject));
	}

	void Scene::RemoveGameObject(core::GameObject& object)
	{
		object.MarkForDeletion();
	}

	void Scene::CleanupGameObjects()
	{
		std::erase_if(m_pObjects,
			[](const auto& pObj)
			{
				return pObj->IsPendingDeletion();
			});
	}

	void Scene::RemoveAllGameObjects()
	{
		m_pObjects.clear();
	}

	void Scene::FlushPendingObjects()
	{
		if (m_pPendingObjects.empty())
		{
			return;
		}

		// Allows adding new objects during the flush without iterator invalidation issues, they will be flushed in the next frame.
		auto p_new_objects = std::move(m_pPendingObjects);
		m_pPendingObjects.clear();

		const size_t new_index_start = m_pObjects.size();

		m_pObjects.reserve(m_pObjects.size() + p_new_objects.size());
		for (auto& object : p_new_objects)
		{
			m_pObjects.emplace_back(std::move(object));
		}

		for (size_t index = new_index_start; index < m_pObjects.size(); ++index)
		{
			m_pObjects[index]->InitializeLinkage();
		}

		for (size_t index = new_index_start; index < m_pObjects.size(); ++index)
		{
			m_pObjects[index]->InitializeState();
		}
	}
}
