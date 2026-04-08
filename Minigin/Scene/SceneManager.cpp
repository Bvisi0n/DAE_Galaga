#include "Minigin/Scene/Scene.h"
#include "Minigin/Scene/SceneManager.h"

namespace dae::scenes
{
	Scene& SceneManager::CreateScene()
	{
		m_scenes.emplace_back( new Scene() );
		return *m_scenes.back();
	}

	void SceneManager::Update( const float deltaTime )
	{
		for ( auto& scene : m_scenes )
		{
			scene->Update( deltaTime );
		}
	}

	void SceneManager::Render()
	{
		for ( const auto& scene : m_scenes )
		{
			scene->Render();
		}
	}
}
