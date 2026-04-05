#ifndef COMPONENT_H
#define COMPONENT_H

namespace dae::core
{
    class GameObject;

    class Component
    {
    public:
        explicit Component(GameObject* pOwner) noexcept
            : m_pOwner(pOwner) {}

        virtual ~Component() = default;

        Component(const Component&)            = delete;
        Component(Component&&)                 = delete;
        Component& operator=(const Component&) = delete;
        Component& operator=(Component&&)      = delete;

        virtual void InitializeLinkage() = 0;
        virtual void InitializeState() = 0;

        virtual void Update(const float deltaTime) = 0;

        void MarkForDeletion() noexcept
        {
            m_IsPendingDeletion = true;
        }

        [[nodiscard]] bool IsPendingDeletion() const noexcept
        {
            return m_IsPendingDeletion;
        }

    protected:
        [[nodiscard]] GameObject* GetOwner() const noexcept
        {
            return m_pOwner;
        }

    private:
        GameObject* m_pOwner;
        bool m_IsPendingDeletion{ false };
    };
}
#endif
