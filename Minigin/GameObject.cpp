#include <string>

#include "GameObject.h"
#include "ResourceManager.h"
#include "Renderer.h"

dae::GameObject::~GameObject() = default;

void dae::GameObject::Update(const float deltaTime)
{
	for (auto& comp : m_components) comp->Update(deltaTime);
}

void dae::GameObject::Render() const
{
	for (const auto& comp : m_components) comp->Render();
}

void dae::GameObject::SetPosition(float x, float y)
{
	m_transform.SetPosition(x, y, 0.0f);
}
