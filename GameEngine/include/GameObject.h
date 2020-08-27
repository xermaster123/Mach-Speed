#pragma once
#include "Component.h"
#include <unordered_map>
#include <typeindex>

class GameObject
{
public:
	virtual ~GameObject()
	{
		// Delete the component pointers from memory when the object is deconstructed
		for (auto it = m_components.begin(); it != m_components.end(); ++it)
		{
			delete (it->second);
		}
		m_components.clear();
	}

	template <typename T>
	T* getComponent()
	{
		auto iter = m_components.find(typeid(T));

		if (iter != std::end(m_components))
		{
			// if found dynamic cast the component pointer and return it
			return dynamic_cast<T*>(iter->second);
		}

		// return null if we don't have a component of that type
		return nullptr;
	}

	template <typename T>
	void addComponent(T* comp)
	{
		// add the component to unoreder map with hash of its typeid
		m_components[typeid(T)] = comp;
	}

	template <typename T>
	void deleteComponent(T* comp)
	{
		auto iter = m_components.find(typeid(T));

		if (iter != std::end(m_components))
		{
			// if found dynamic cast the component pointer and return it
			m_components.erase(iter);
		}
	}

	virtual void OnUpdate(float dt) { }
	virtual void OnMessage(const std::string m) { }

	bool m_enabled = true;

private:
	std::unordered_map<std::type_index, Component*> m_components;
};
