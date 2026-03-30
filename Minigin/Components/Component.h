#ifndef COMPONENT_H
#define COMPONENT_H

namespace dae
{
    class GameObject;

    class Component
    {
    public:
        explicit Component(GameObject* pOwner) : m_pOwner(pOwner) {}
        virtual ~Component() = default;

        Component(const Component&)            = delete;
        Component(Component&&)                 = delete;
        Component& operator=(const Component&) = delete;
        Component& operator=(Component&&)      = delete;

        virtual void Update(const float deltaTime) = 0;
        void MarkForDeletion() { m_IsPendingDeletion = true; }
        [[nodiscard]] bool IsPendingDeletion() const { return m_IsPendingDeletion; }

    protected:
        [[nodiscard]] GameObject* GetOwner() const { return m_pOwner; }

    private:
        GameObject* m_pOwner;
        bool m_IsPendingDeletion{ false };
    };
}
#endif
