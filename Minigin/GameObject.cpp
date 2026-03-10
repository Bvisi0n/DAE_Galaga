#include <string>

#include "Singletons/Renderer.h"
#include "Singletons/ResourceManager.h"
#include "GameObject.h"
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
	if (IsChild(parent) || parent == this || m_pParent == parent)
	{
		return;
	}

	// Update local position to maintain world position if needed
	if (parent == nullptr)
	{
		SetLocalPosition(GetGlobalPosition());
	}
	else
	{
		if (keepWorldPosition)
		{
			SetLocalPosition(GetGlobalPosition() - parent->GetGlobalPosition());
		}
		SetPositionDirty();
	}

	// Detach from current parent
	if (m_pParent)
	{
		m_pParent->RemoveChild(this);
	}

	// Attach to new parent
	m_pParent = parent;
	if (m_pParent)
	{
		m_pParent->AddChild(this);
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

dae::Transform& dae::GameObject::GetGlobalPosition()
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
		if (m_pParent == nullptr)
		{
			m_worldPosition = m_localPosition;
		}
		else
		{
			m_worldPosition = m_pParent->GetGlobalPosition() + m_localPosition;
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
