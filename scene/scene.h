// Scene.h
#pragma once
#include "common.h"
#include "gameobject.h"
#include "model.h"
#include "program.h"

CLASS_PTR(Scene)
class Scene {
public:
    virtual ~Scene() = default;

    virtual void Init() = 0;
    virtual void Update(float deltaTime) = 0;
    virtual void Render() = 0;
    virtual void Shutdown() = 0;

    std::vector<GameObjectPtr> GetAllObjects() const {
        return m_objects;
    }

    struct SpawnRequest {
        std::function<void()> fn;
    };

    template<typename T, typename... Args>
    void RequestSpawn(Args&&... args) {
        //using Tup = std::tuple<std::decay_t<Args>...>;
        m_pendingSpawn.push_back({
            [this, tup = std::make_tuple(std::forward<Args>(args)...)]() mutable {
                // 실제 스폰은 여기서
                std::apply([this](auto&&... unpacked) {
                    auto obj = Spawn<T>(std::forward<decltype(unpacked)>(unpacked)...);
                    // Spawn() 안에서 Init, push_back 모두 처리한다고 가정
                }, std::move(tup));
            }
        });
    }

    template<typename T, typename... Args>
    std::shared_ptr<T> Spawn(Args&&... args) {
        auto obj = T::Create(std::forward<Args>(args)...);
        obj->SetScene(this);
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
    std::vector<SpawnRequest> m_pendingSpawn;
};
