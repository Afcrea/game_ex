// Scene.h
#pragma once
#include "common.h"
#include "gameobject.h"


CLASS_PTR(Scene)
class Scene {
public:
    virtual ~Scene() = default;

    virtual void Init() = 0;
    virtual void Update(float deltaTime) = 0;
    virtual void Render() = 0;
    virtual void Shutdown() = 0;

    template<typename T, typename... Args>
    std::shared_ptr<T> Spawn(Args&&... args) {
        auto obj = T::Create(std::forward<Args>(args)...);
        obj->SetScene(this);
        obj->Init();
        m_objects.push_back(obj);
        return obj;
    }

    // 즉시 삭제하지 않고, 안전하게 삭제 대기 리스트로 옮깁니다.
    void Destroy(GameObject* obj) {
        // m_objects 에서 raw pointer 가 일치하는 shared_ptr 를 찾습니다.
        auto it = std::find_if(
            m_objects.begin(), m_objects.end(),
            [obj](const GameObjectPtr& ptr) { return ptr.get() == obj; }
        );
        if (it != m_objects.end()) {
            (*it)->MarkPendingDestroy();
            m_pendingRemove.push_back(*it);
        }
    }

protected:
    std::vector<GameObjectPtr> m_objects;
    std::vector<GameObjectPtr> m_pendingRemove;
};
