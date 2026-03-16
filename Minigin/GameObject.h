#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include <memory>
#include <string>
#include <vector>

#include "Components/Component.h"
#include "Transform.h"

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

		// DAEN: Should check for doubles of same type, Text and Texture cannot coexist.
		// DAEN: Use concepts.
		template <typename T, typename... Args>
		T* AddComponent(Args&&... args) {
			auto component = std::make_unique<T>(this, std::forward<Args>(args)...);
			T* ptr = component.get();
			m_pComponents.push_back(std::move(component));
			return ptr;
		}

		// DAEN: Use concepts.
		template <typename T>
		bool HasComponent() const {
			return GetComponent<T>() != nullptr;
		}

		// DAEN: Ensure GetComponent isn't called in the hotpath.
		// DAEN: Use concepts.
		template <typename T>
		T* GetComponent() const {
			for (const auto& comp : m_pComponents) {
				T* ptr = dynamic_cast<T*>(comp.get());
				if (ptr) return ptr;
			}
			return nullptr;
		}

		// DAEN: Should use a flag to mark for deletion.
		// DAEN: Use concepts.
		template <typename T>
		void RemoveComponent() {
			m_pComponents.erase(std::remove_if(m_pComponents.begin(), m_pComponents.end(),
				[](const std::unique_ptr<Component>& comp)
				{
					return dynamic_cast<T*>(comp.get()) != nullptr;
				}), m_pComponents.end());
		}

		void SetParent(GameObject* pParent, bool keepWorldPosition);
		const std::vector<GameObject*>& GetChildren() const;
		
		void SetLocalPosition(float x, float y);
		void SetLocalPosition(Transform pos);
		Transform& GetGlobalPosition();
		Transform& GetLocalPosition();

	private:
		GameObject* m_pParent{ nullptr };
		std::vector<GameObject*> m_pChildren;
		std::vector<std::unique_ptr<Component>> m_pComponents;
		Transform m_localPosition{};
		Transform m_globalPosition{};
		bool m_positionIsDirty{ true };

		bool IsChild(GameObject* pCandidate);
		void RemoveChild(GameObject* pParent);
		void AddChild(GameObject* pParent);

		void UpdateWorldPosition();
		void SetPositionDirty();
	};
}
#endif
