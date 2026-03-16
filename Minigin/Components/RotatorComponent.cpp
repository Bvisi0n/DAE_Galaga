#include <numbers>

#include "Components/RotatorComponent.h"
#include "GameObject.h"

dae::RotatorComponent::RotatorComponent(GameObject* pOwner, float range, float speed)
    : Component(pOwner), m_range(range), m_rotationSpeed(speed) {}

void dae::RotatorComponent::Update(float deltaTime)
{
    m_angle += m_rotationSpeed * deltaTime;

    const float double_pi{ std::numbers::pi_v<float> *2.0f };

    if (m_angle > double_pi)
    {
        m_angle -= double_pi;
    }
    else if (m_angle < double_pi)
    {
        m_angle += double_pi;
    }

    // DAEN: RotatorComponent should support ellipses.

    // x = cos(a) * r, y = sin(a) * r
    GetOwner()->SetLocalPosition(cos(m_angle) * m_range, sin(m_angle) * m_range);
}
