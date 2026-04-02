#ifndef ROTATORCOMPONENT_H
#define ROTATORCOMPONENT_H

#include <glm/glm.hpp>

#include "Minigin/Core/Component.h"

namespace dae
{
    class core::GameObject;

    // Allows a GameObject to rotate around a parent in a circular path.
    class RotatorComponent final : public core::Component
    {
    public:
        RotatorComponent(core::GameObject* pOwner, float range, float speed);
        ~RotatorComponent() override = default;

        void Initialize() override {}
        void Update(const float deltaTime) override;

    private:
        float m_angle{ 0.0f };
        float m_range{ 0.0f };
        float m_rotationSpeed{ 0.0f };
    };
}
#endif
