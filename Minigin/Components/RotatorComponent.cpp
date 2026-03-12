#include <numbers>

#include "Components/RotatorComponent.h"
#include "GameObject.h"

dae::RotatorComponent::RotatorComponent(GameObject* pOwner, float range, float speed)
    : BaseComponent(pOwner), m_Range(range), m_RotationSpeed(speed) {}

void dae::RotatorComponent::Update(float deltaTime)
{
    m_Angle += m_RotationSpeed * deltaTime;

    const float double_pi{ std::numbers::pi_v<float> *2.0f };

    if (m_Angle > double_pi)
    {
        m_Angle -= double_pi;
    }
    else if (m_Angle < double_pi)
    {
        m_Angle += double_pi;
    }

    // DAEN: RotatorComponent should support ellipses.

    // x = cos(a) * r, y = sin(a) * r
    GetOwner()->SetLocalPosition(cos(m_Angle) * m_Range, sin(m_Angle) * m_Range);
}
