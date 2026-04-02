#include "Minigin/Scene/Scene.h"
#include "Minigin/Scene/SceneManager.h"

dae::Scene& dae::SceneManager::CreateScene()
{
	m_pScenes.emplace_back(new Scene());
	return *m_pScenes.back();
}

void dae::SceneManager::Update(const float deltaTime)
{
	for(auto& scene : m_pScenes)
	{
		scene->Update(deltaTime);
	}
}

void dae::SceneManager::Render()
{
	for (const auto& scene : m_pScenes)
	{
		scene->Render();
	}
}
