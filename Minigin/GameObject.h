#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include <cassert>
#include <concepts>
#include <memory>
#include <string>
#include <vector>

#include "Components/Component.h"

namespace dae
{
	class Texture2D;
	class IRenderable;

	template<typename T>
	concept IsComponent = std::derived_from<T, Component>;

	template<typename T>
	concept IsRenderable = std::derived_from<T, Component>&& std::derived_from<T, IRenderable>;

	class GameObject final
	{
		// TODO N: Fit entire object in 64 bytes.
	public:
		GameObject() = default;
		~GameObject();

		GameObject(const GameObject& other)			   = delete;
		GameObject(GameObject&& other)				   = delete;
		GameObject& operator=(const GameObject& other) = delete;
		GameObject& operator=(GameObject&& other)	   = delete;

		void Update(const float deltaTime);
		void Render() const;

		// TODO N: Should check for doubles of same type.
		template <IsComponent T, typename... Args>
			requires (!IsRenderable<T>)
		T* AddComponent(Args&&... args)
		{
			auto component = std::make_unique<T>(this, std::forward<Args>(args)...);
			T* ptr = component.get();
			m_pComponents.push_back(std::move(component));
			return ptr;
		}

		template <IsRenderable T, typename... Args>
		T* AddComponent(Args&&... args)
		{
			if (m_pRenderable)
			{
				assert(!m_pRenderable && "GameObject can only have one IRenderable.");
				return nullptr;
			}

			auto component = std::make_unique<T>(this, std::forward<Args>(args)...);
			T* ptr = component.get();

			m_pRenderable = ptr;
			m_pComponents.push_back(std::move(component));
			return ptr;
		}

		template <IsComponent T>
		bool HasComponent() const
		{
			return GetComponent<T>() != nullptr;
		}

		template <IsComponent T>
			requires (!IsRenderable<T>)
		T* GetComponent() const
		{
			for (const auto& comp : m_pComponents)
			{
				T* ptr = dynamic_cast<T*>(comp.get());
				if (ptr)
				{
					return ptr;
				}
			}
			return nullptr;
		}

		template <IsRenderable T>
		T* GetComponent() const
		{
			if (!m_pRenderable)
			{
				assert(m_pRenderable && "Attempting to get an IRenderable from a GameObject that has none.");
				return nullptr;
			}
			return dynamic_cast<T*>(m_pRenderable);
		}

		// TODO N: Should use a flag to mark for deletion.
		template <IsComponent T>
			requires (!IsRenderable<T>)
		void RemoveComponent()
		{
			m_pComponents.erase(std::remove_if(m_pComponents.begin(), m_pComponents.end()
				, [](const std::unique_ptr<Component>& comp)
				{
					return dynamic_cast<T*>(comp.get()) != nullptr;
				}), m_pComponents.end());
		}

		template <IsRenderable T>
		void RemoveComponent()
		{
			if (m_pRenderable && dynamic_cast<T*>(m_pRenderable))
			{
				m_pComponents.erase(std::remove_if(m_pComponents.begin(), m_pComponents.end()
					, [this](const std::unique_ptr<Component>& comp)
					{
						return dynamic_cast<IRenderable*>(comp.get()) == m_pRenderable;
					}), m_pComponents.end());
				m_pRenderable = nullptr;
			}
		}

		void SetParent(GameObject* pParent, bool keepWorldPosition);
		GameObject* GetParent() const;
		const std::vector<GameObject*>& GetChildren() const;
		
	private:
		GameObject* m_pParent{ nullptr };
		std::vector<GameObject*> m_pChildren;

		std::vector<std::unique_ptr<Component>> m_pComponents;
		IRenderable* m_pRenderable{ nullptr };

		bool IsChild(GameObject* pCandidate);
		void RemoveChild(GameObject* pParent);
		void AddChild(GameObject* pParent);
	};
}
#endif
