#include <string>

#include "GameObject.h"
#include "ResourceManager.h"
#include "Renderer.h"
#include "Transform.h"

dae::GameObject::~GameObject() = default;

void dae::GameObject::Update(const float deltaTime)
{
	for (auto& comp : m_components) comp->Update(deltaTime);
}

void dae::GameObject::Render() const
{
	for (const auto& comp : m_components) comp->Render();
}

void dae::GameObject::SetParent(GameObject* parent, bool keepWorldPosition)
{
	if (IsChild(parent) || parent == this || m_parent == parent)
	{
		return;
	}

	if (parent == nullptr)
	{
		SetLocalPosition(GetWorldPosition());
	}
	else
	{
		if (keepWorldPosition)
		{
			SetLocalPosition(GetWorldPosition() - parent->GetWorldPosition());
		}
		SetPositionDirty();
	}

	if (m_parent)
	{
		m_parent->RemoveChild(this);
	}

	m_parent = parent;

	if (m_parent)
	{
		m_parent->AddChild(this);
	}
}

bool dae::GameObject::IsChild(GameObject* parent)
{
	return parent == this;
}

void dae::GameObject::RemoveChild(GameObject* /*parent*/)
{
}

void dae::GameObject::AddChild(GameObject* /*parent*/)
{
}

void dae::GameObject::SetLocalPosition(float x, float y)
{
	m_localPosition.SetPosition(x, y, 0.0f);
	SetPositionDirty();
}

void dae::GameObject::SetLocalPosition(Transform pos)
{
	m_localPosition = pos;
	SetPositionDirty();
}

dae::Transform& dae::GameObject::GetWorldPosition()
{
	if (m_positionIsDirty)
	{
		UpdateWorldPosition();
	}

	return m_worldPosition;
}

void dae::GameObject::UpdateWorldPosition()
{
	if (m_positionIsDirty)
	{
		if (m_parent == nullptr)
		{
			m_worldPosition = m_localPosition;
		}
		else
		{
			m_worldPosition = m_parent->GetWorldPosition() + m_localPosition;
		}
	}

	m_positionIsDirty = false;
}

void dae::GameObject::SetPositionDirty()
{
	m_positionIsDirty = true;
	for (const auto& child : m_children)
	{
		child->SetPositionDirty();
	}
}

dae::Transform& dae::GameObject::GetLocalPosition()
{
	return m_localPosition;
}
