#ifndef ROTATORCOMPONENT_H
#define ROTATORCOMPONENT_H

#include <glm/glm.hpp>

#include "BaseComponent.h"

namespace dae
{
    class GameObject;

    class RotatorComponent final : public BaseComponent
    {
    public:
        RotatorComponent(GameObject* pOwner, float range, float speed);
        ~RotatorComponent() override = default;

        void Update(float deltaTime) override;
        void Render() const override {};

    private:
        float m_Angle{ 0.0f };
        float m_Range{ 0.0f };
        float m_RotationSpeed{ 0.0f };
    };
}

#endif
