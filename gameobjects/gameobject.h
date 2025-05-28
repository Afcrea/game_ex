#pragma once
#include "common.h"
#include "component/component.h"
#include "camera.h"
#include <unordered_map>
#include <typeindex>

class Scene;

CLASS_PTR(GameObject)
class GameObject {
public:
    virtual ~GameObject() = default;

    virtual void Init() {
        for (auto& [type, comp] : m_components)
            comp->Init();
    }

    virtual void Update(float deltaTime) {
        for (auto& [type, comp] : m_components)
            comp->Update(deltaTime);
    }

    virtual void Render(CameraPtr camera) {
        for (auto& [type, comp] : m_components)
            comp->Render(camera);
    }

    virtual void Shutdown() {
        for (auto& [type, comp] : m_components)
            comp->Shutdown();
    }

    // 컴포넌트 추가
    template<typename T, typename... Args>
    std::shared_ptr<T> AddComponent(Args&&... args) {
        auto comp = T::Create(std::forward<Args>(args)...);
        comp->SetOwner(this);
        m_components[typeid(T)] = comp;
        return comp;
    }

    // 컴포넌트 얻기
    template<typename T>
    std::shared_ptr<T> GetComponent() {
        auto it = m_components.find(typeid(T));
        if (it != m_components.end()) {
            return std::static_pointer_cast<T>(it->second);
        }
        return nullptr;
    }

    // 컴포넌트 제거
    template<typename T>
    void RemoveComponent() {
        auto it = m_components.find(typeid(T));
        if (it != m_components.end()) {
            it->second->Shutdown();
            m_components.erase(it);
        }
    }

    const std::unordered_map<std::type_index, ComponentPtr>& GetAllComponents() const { return m_components; }
    const std::string& GetName() const { return m_name; }
    void SetName(const std::string& name) { m_name = name; }
    
    void SetScene(Scene* scene) { m_scene = scene; }
    Scene* GetScene() const    { return m_scene; }

    void MarkPendingDestroy() { m_pendingDestroy = true; }
    bool IsPendingDestroy() const { return m_pendingDestroy; }
protected:
    std::unordered_map<std::type_index, ComponentPtr> m_components;
    std::string m_name;
    Scene* m_scene = nullptr;
    bool m_pendingDestroy = false;
};