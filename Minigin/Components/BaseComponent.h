#ifndef BASECOMPONENT_H
#define BASECOMPONENT_H

namespace dae
{
    class GameObject;

    class BaseComponent
    {
    public:
        BaseComponent(GameObject* pOwner) : m_pOwner(pOwner) {}
        virtual ~BaseComponent() = default;

        BaseComponent(const BaseComponent&)            = delete;
        BaseComponent(BaseComponent&&)                 = delete;
        BaseComponent& operator=(const BaseComponent&) = delete;
        BaseComponent& operator=(BaseComponent&&)      = delete;

        virtual void Update(const float deltaTime) = 0;
        virtual void Render() const = 0;

    protected:
        GameObject* GetOwner() const { return m_pOwner; }

    private:
        GameObject* m_pOwner;
    };
}
#endif
