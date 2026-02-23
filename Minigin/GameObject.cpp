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
	// Prevent circular parenting
	if (IsChild(parent) || parent == this || m_parent == parent)
	{
		return;
	}

	// Update local position to maintain world position if needed
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

	// Detach from current parent
	if (m_parent)
	{
		m_parent->RemoveChild(this);
	}

	// Attach to new parent
	m_parent = parent;
	if (m_parent)
	{
		m_parent->AddChild(this);
	}
}

const std::vector<dae::GameObject*>& dae::GameObject::GetChildren() const
{
	return m_children;
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

bool dae::GameObject::IsChild(GameObject* parent)
{
	return parent == this;
}

void dae::GameObject::RemoveChild(GameObject* pChild)
{
	m_children.erase(std::remove(m_children.begin(), m_children.end(), pChild), m_children.end());
}

void dae::GameObject::AddChild(GameObject* pChild)
{
	m_children.emplace_back(pChild);
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
