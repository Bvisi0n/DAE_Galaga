#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include <memory>
#include <string>

#include "Components/BaseComponent.h"
#include "Transform.h"

namespace dae
{
	// Forward declarations
	class Texture2D;

	class GameObject 
	{
	public:
		GameObject() = default;
		virtual ~GameObject();

		GameObject(const GameObject& other)			   = delete;
		GameObject(GameObject&& other)				   = delete;
		GameObject& operator=(const GameObject& other) = delete;
		GameObject& operator=(GameObject&& other)	   = delete;

		virtual void Update([[maybe_unused]] const float deltaTime);
		virtual void Render() const;

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

		void SetTexture(const std::string& filename);
		void SetPosition(float x, float y);

		Transform& GetTransform() { return m_transform; };

	private:
		std::vector<std::unique_ptr<BaseComponent>> m_components;
		Transform m_transform{};
		std::shared_ptr<Texture2D> m_texture{};
	};
}
#endif
