#include <algorithm>

#include "Scene.h"

using namespace dae;

void Scene::Update(const float deltaTime)
{
	for(auto& object : m_pObjects)
	{
		object->Update(deltaTime);
	}
}

void Scene::Render() const
{
	for (const auto& object : m_pObjects)
	{
		object->Render();
	}
}

void Scene::Add(std::unique_ptr<GameObject> pObject)
{
	assert(object != nullptr && "Cannot add a null GameObject to the scene.");
	m_pObjects.emplace_back(std::move(pObject));
}

void Scene::Remove(const GameObject& object)
{
	m_pObjects.erase(
		std::remove_if(
			m_pObjects.begin(),
			m_pObjects.end(),
			[&object](const auto& ptr) { return ptr.get() == &object; }
		),
		m_pObjects.end()
	);
}

void Scene::RemoveAll()
{
	m_pObjects.clear();
}
