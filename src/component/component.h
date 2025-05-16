#pragma once
#include "common.h"
#include "camera.h"

class GameObject;

CLASS_PTR(Component)
class Component {
public:
    virtual ~Component() = default;
    virtual void Init() {}
    virtual void Update(float deltaTime) {}
    virtual void Render(CameraPtr camera) {}
    virtual void Shutdown() {}
    virtual void OnTriggerEnter(GameObject* other) {}
    
    // 생포인터로
    virtual void SetOwner(GameObject* owner) { m_owner = owner; }
    GameObject* GetOwner() const { return m_owner; }

protected:
    GameObject* m_owner = nullptr; //
};