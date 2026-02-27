#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include <memory>
#include <string>
#include <vector>

#include "Components/BaseComponent.h"
#include "Components/TransformComponent.h"

namespace dae
{
	class Texture2D;

	class GameObject final
	{
	public:
		GameObject() = default;
		~GameObject();

		GameObject(const GameObject& other)			   = delete;
		GameObject(GameObject&& other)				   = delete;
		GameObject& operator=(const GameObject& other) = delete;
		GameObject& operator=(GameObject&& other)	   = delete;

		void Update(const float deltaTime);
		void Render() const;

		template <typename T, typename... Args>
		T* AddComponent(Args&&... args) {
			auto component = std::make_unique<T>(this, std::forward<Args>(args)...);
			T* ptr = component.get();
			m_components.push_back(std::move(component));
			return ptr;
		}

		template <typename T>
		bool HasComponent() const {
			return GetComponent<T>() != nullptr;
		}

		template <typename T>
		T* GetComponent() const {
			for (const auto& comp : m_components) {
				T* ptr = dynamic_cast<T*>(comp.get());
				if (ptr) return ptr;
			}
			return nullptr;
		}

		template <typename T>
		void RemoveComponent() {
			m_components.erase(std::remove_if(m_components.begin(), m_components.end(),
				[](const std::unique_ptr<BaseComponent>& comp)
				{
					return dynamic_cast<T*>(comp.get()) != nullptr;
				}), m_components.end());
		}

		void SetParent(GameObject* pParent, bool keepWorldPosition);
		const std::vector<GameObject*>& GetChildren() const;
		
		void SetLocalPosition(float x, float y);
		void SetLocalPosition(TransformComponent pos);
		TransformComponent& GetGlobalPosition();

	private:
		GameObject* m_parent{ nullptr };
		std::vector<GameObject*> m_children;
		std::vector<std::unique_ptr<BaseComponent>> m_components;
		TransformComponent m_localPosition{};
		TransformComponent m_worldPosition{};
		bool m_positionIsDirty{ true };

		bool IsChild(GameObject* pCandidate);
		void RemoveChild(GameObject* pParent);
		void AddChild(GameObject* pParent);

		void UpdateWorldPosition();
		void SetPositionDirty();
		TransformComponent& GetLocalPosition();
	};
}
#endif
