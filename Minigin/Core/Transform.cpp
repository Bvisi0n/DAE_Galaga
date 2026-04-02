#include <glm/gtc/matrix_transform.hpp>

#include "Minigin/Core/GameObject.h"
#include "Minigin/Core/Transform.h"

namespace dae::core
{
    Transform::Transform(GameObject* pOwner, float xPos, float yPos, float xScale, float yScale)
        : m_pOwner(pOwner)
        , m_localMatrix(1.f)
        , m_worldMatrix(1.f)
        , m_worldPosition(xPos, yPos, 0.f)
        , m_localPosition(xPos, yPos, 0.f)
        , m_localScale(xScale, yScale, 1.f)
        , m_localRotation(0.f) {}

    void Transform::SetLocalPosition(const glm::vec3& pos)
    {
        m_localPosition = pos;
        SetDirty();
    }

    void Transform::SetLocalPosition(float x, float y, float z)
    {
        SetLocalPosition(glm::vec3{ x, y, z });
    }

    const glm::vec3& Transform::GetLocalPosition() const
    {
        return m_localPosition;
    }

    void Transform::SetLocalRotation(float angle)
    {
        m_localRotation = angle;
        SetDirty();
    }

    float Transform::GetLocalRotation() const
    {
        return m_localRotation;
    }

    void Transform::SetLocalScale(const glm::vec3& scale)
    {
        m_localScale = scale;
        SetDirty();
    }

    void Transform::SetLocalScale(float x, float y, float z)
    {
        SetLocalScale(glm::vec3{ x, y, z });
    }

    const glm::vec3& Transform::GetLocalScale() const
    {
        return m_localScale;
    }

    const glm::mat4& Transform::GetWorldMatrix() const
    {
        return m_worldMatrix;
    }

    const glm::vec3& Transform::GetWorldPosition() const
    {
        return m_worldPosition;
    }

    void Transform::UpdateWorldMatrix(const glm::mat4& parentWorldMatrix)
    {
        UpdateLocalMatrix();
        m_worldMatrix = parentWorldMatrix * m_localMatrix;
        m_worldPosition = glm::vec3(m_worldMatrix[3]);

        for (const auto& child : m_pOwner->GetChildren())
        {
            child->GetTransform().UpdateWorldMatrix(m_worldMatrix);
        }
    }

    void Transform::SetDirty()
    {
        if (m_isDirty)
        {
            return;
        }
        else
        {
            m_isDirty = true;
            for (const auto& child : m_pOwner->GetChildren())
            {
                child->GetTransform().SetDirty();
            }
        }
    }

    void Transform::UpdateLocalMatrix()
    {
        if (!m_isDirty) return;

        // Don't change the order! (TRS)
        m_localMatrix = glm::translate(glm::mat4(1.0f), m_localPosition);
        m_localMatrix = glm::rotate(m_localMatrix, m_localRotation, { 0, 0, 1 });
        m_localMatrix = glm::scale(m_localMatrix, m_localScale);

        m_isDirty = false;
    }
}
