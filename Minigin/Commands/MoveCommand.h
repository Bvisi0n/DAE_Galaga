#ifndef MOVECOMMAND_H
#define MOVECOMMAND_H

#include <glm/glm.hpp>

#include "Commands/Command.h"
#include "Components/TransformComponent.h"
#include "GameObject.h"

// DAEL: Split into .h & .cpp.

namespace dae
{
    class MoveCommand final : public Command
    {
    public:
        MoveCommand(GameObject* pGameObject, const glm::vec2& direction, float speed)
            : m_pGameObject(pGameObject)
            , m_direction(direction)
            , m_speed(speed) {}

        void Execute(const float deltaTime) override
        {
            // DAEN: Diagonal movement is currently double speed.
            auto* pTransform = m_pGameObject->GetComponent<dae::TransformComponent>();
            auto position = pTransform->GetLocalPosition();

            // DAEN: Use vector math.
            position.x += m_direction.x * m_speed * deltaTime;
            position.y += m_direction.y * m_speed * deltaTime;

            pTransform->SetLocalPosition(position);
        }

    private:
        GameObject* m_pGameObject;
        glm::vec2 m_direction;
        float m_speed;
    };
}
#endif
