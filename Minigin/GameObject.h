#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include <memory>
#include <string>
#include <vector>

#include "Components/Component.h"

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

		// TODO N: Should check for doubles of same type, Text and Texture cannot coexist.
		// TODO N: Use concepts.
		template <typename T, typename... Args>
		T* AddComponent(Args&&... args)
		{
			auto component = std::make_unique<T>(this, std::forward<Args>(args)...);
			T* ptr = component.get();
			m_pComponents.push_back(std::move(component));
			return ptr;
		}

		// TODO N: Use concepts.
		template <typename T>
		bool HasComponent() const
		{
			return GetComponent<T>() != nullptr;
		}

		// TODO N: Ensure GetComponent isn't called in the hotpath. (Transform, Texture, Text...)
		// TODO N: Use concepts.
		template <typename T>
		T* GetComponent() const
		{
			for (const auto& comp : m_pComponents)
			{
				T* ptr = dynamic_cast<T*>(comp.get());
				if (ptr) return ptr;
			}
			return nullptr;
		}

		// TODO N: Should use a flag to mark for deletion.
		// TODO N: Use concepts.
		template <typename T>
		void RemoveComponent()
		{
			m_pComponents.erase(std::remove_if(m_pComponents.begin(), m_pComponents.end(),
				[](const std::unique_ptr<Component>& comp)
				{
					return dynamic_cast<T*>(comp.get()) != nullptr;
				}), m_pComponents.end());
		}

		void SetParent(GameObject* pParent, bool keepWorldPosition);
		GameObject* GetParent() const;
		const std::vector<GameObject*>& GetChildren() const;
		
	private:
		GameObject* m_pParent{ nullptr };
		std::vector<GameObject*> m_pChildren;
		std::vector<std::unique_ptr<Component>> m_pComponents;

		bool IsChild(GameObject* pCandidate);
		void RemoveChild(GameObject* pParent);
		void AddChild(GameObject* pParent);
	};
}
#endif
