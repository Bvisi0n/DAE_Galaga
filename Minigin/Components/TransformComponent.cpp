#include <glm/gtc/matrix_transform.hpp>

#include "Components/TransformComponent.h"
#include "GameObject.h"

namespace dae
{
    TransformComponent::TransformComponent(GameObject* pOwner, float xPos, float yPos, float xScale, float yScale)
        : Component(pOwner)
        , m_localMatrix(1.f)
        , m_worldMatrix(1.f)
        , m_worldPosition(xPos, yPos, 0.f)
        , m_localPosition(xPos, yPos, 0.f)
        , m_localScale(xScale, yScale, 1.f)
        , m_localRotation(0.f) {}

    void TransformComponent::SetLocalPosition(const glm::vec3& pos)
    {
        m_localPosition = pos;
        SetDirty();
    }

    void TransformComponent::SetLocalPosition(float x, float y, float z)
    {
        SetLocalPosition(glm::vec3{ x, y, z });
    }

    const glm::vec3& TransformComponent::GetLocalPosition() const
    {
        return m_localPosition;
    }

    void TransformComponent::SetLocalRotation(float angle)
    {
        m_localRotation = angle;
        SetDirty();
    }

    float TransformComponent::GetLocalRotation() const
    {
        return m_localRotation;
    }

    void TransformComponent::SetLocalScale(const glm::vec3& scale)
    {
        m_localScale = scale;
        SetDirty();
    }

    void TransformComponent::SetLocalScale(float x, float y, float z)
    {
        SetLocalScale(glm::vec3{ x, y, z });
    }

    const glm::vec3& TransformComponent::GetLocalScale() const
    {
        return m_localScale;
    }

    const glm::mat4& TransformComponent::GetWorldMatrix() const
    {
        return m_worldMatrix;
    }

    const glm::vec3& dae::TransformComponent::GetWorldPosition()
    {
        return m_worldPosition;
    }

    void TransformComponent::UpdateWorldMatrix(const glm::mat4& parentWorldMatrix)
    {
        UpdateLocalMatrix();
        m_worldMatrix = parentWorldMatrix * m_localMatrix;
        m_worldPosition = glm::vec3(m_worldMatrix[3]);

        for (const auto& child : GetOwner()->GetChildren())
        {
            if (auto childTransform = child->GetComponent<TransformComponent>())
            {
                childTransform->UpdateWorldMatrix(m_worldMatrix);
            }
        }
    }

    void TransformComponent::SetDirty()
    {
        m_isDirty = true;
        // TODO H: What if there is no TransformComponent?
        for (const auto& child : GetOwner()->GetChildren())
        {
            child->GetComponent<TransformComponent>()->SetDirty();
        }
    }

    void TransformComponent::UpdateLocalMatrix()
    {
        if (!m_isDirty) return;

        m_localMatrix = glm::translate(glm::mat4(1.0f), m_localPosition);
        m_localMatrix = glm::rotate(m_localMatrix, m_localRotation, { 0, 0, 1 });
        m_localMatrix = glm::scale(m_localMatrix, m_localScale);

        m_isDirty = false;
    }
}
