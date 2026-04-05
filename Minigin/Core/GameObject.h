#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include <algorithm>
#include <cassert>
#include <concepts>
#include <memory>
#include <string>
#include <vector>

#include "Minigin/Core/Component.h"
#include "Minigin/Core/Transform.h"
#include "Minigin/Graphics/IRenderable.h"

namespace dae::graphics
{
	class Texture2D;
}

namespace dae::core
{

	template<typename T>
	concept IsComponent = std::derived_from<T, Component>;

	template<typename T>
	concept IsRenderable = std::derived_from<T, Component> && std::derived_from<T, graphics::IRenderable>;

	class GameObject final
	{
	public:
		GameObject(const float x = 0.f, const float y = 0.f);
		~GameObject();

		GameObject(const GameObject& other)			   = delete;
		GameObject(GameObject&& other)				   = delete;
		GameObject& operator=(const GameObject& other) = delete;
		GameObject& operator=(GameObject&& other)	   = delete;

		void MarkForDeletion();
		[[nodiscard]] bool IsPendingDeletion() const noexcept;

		void InitializeLinkage();
		void InitializeState();
		void Update(const float deltaTime);
		void Render() const;

		template <IsComponent T, typename... Args>
			requires (!IsRenderable<T>)
		T* AddComponent(Args&&... args)
		{
			if (HasComponent<T>())
			{
				assert(!HasComponent<T>() && "GameObject already has a component of this type.");
				return GetComponent<T>();
			}
			else
			{
				auto component = std::make_unique<T>(this, std::forward<Args>(args)...);
				T* ptr = component.get();
				m_pComponents.push_back(std::move(component));
				return ptr;
			}
		}

		template <IsRenderable T, typename... Args>
		T* AddComponent(Args&&... args)
		{
			if (m_pRenderable)
			{
				assert(!m_pRenderable && "GameObject can only have one IRenderable.");
				return nullptr;
			}
			else
			{
				auto component = std::make_unique<T>(this, std::forward<Args>(args)...);
				T* ptr = component.get();

				m_pRenderable = ptr;
				m_pComponents.push_back(std::move(component));
				return ptr;
			}
		}

		template <IsComponent T>
		[[nodiscard]] bool HasComponent() const
		{
			return GetComponent<T>() != nullptr;
		}

		template <IsComponent T>
			requires (!IsRenderable<T>)
		[[nodiscard]] T* GetComponent() const
		{
			for (const auto& comp : m_pComponents)
			{
				T* ptr = dynamic_cast<T*>(comp.get());
				if (ptr && !ptr->IsPendingDeletion())
				{
					return ptr;
				}
			}
			return nullptr;
		}

		template <IsRenderable T>
		[[nodiscard]] T* GetComponent() const
		{
			if (!m_pRenderable)
			{
				return nullptr;
			}
			else
			{
				T* pCastedRenderable = dynamic_cast<T*>(m_pRenderable);

				if (pCastedRenderable && !pCastedRenderable->IsPendingDeletion())
				{
					return pCastedRenderable;
				}
				else
				{
					return nullptr;
				}
			}
		}

		template <IsComponent T>
			requires (!IsRenderable<T>)
		void RemoveComponent()
		{
			for (auto& comp : m_pComponents)
			{
				if (dynamic_cast<T*>(comp.get()))
				{
					comp->MarkForDeletion();
					return;
				}
			}
		}

		template <IsRenderable T>
		void RemoveComponent()
		{
			if (m_pRenderable)
			{
				if (auto casted = dynamic_cast<T*>(m_pRenderable))
				{
					casted->MarkForDeletion();
					m_pRenderable = nullptr;
				}
			}
		}

		void SetParent(GameObject* pParent, bool keepWorldPosition);
		[[nodiscard]] GameObject* GetParent() const;
		[[nodiscard]] const std::vector<GameObject*>& GetChildren() const;

		[[nodiscard]] Transform& GetTransform();
		[[nodiscard]] const Transform& GetTransform() const;

	private:
		GameObject* m_pParent{ nullptr };
		std::vector<GameObject*> m_pChildren;

		std::vector<std::unique_ptr<Component>> m_pComponents;
		graphics::IRenderable* m_pRenderable{ nullptr };

		Transform m_transform;
		bool m_isPendingDeletion{ false };

		bool IsChild(GameObject* pCandidate);
		void AddChild(GameObject* pChild);
		void RemoveChild(GameObject* pChild);

		void CleanupComponents();
	};
}
#endif
