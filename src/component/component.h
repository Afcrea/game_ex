#pragma once
#include "common.h"

class GameObject;

CLASS_PTR(Component)
class Component {
public:
    virtual ~Component() = default;
    virtual void Init() {}
    virtual void Update(double deltaTime) {}
    virtual void Render() {}
    virtual void Shutdown() {}

    // 생포인터로
    virtual void SetOwner(GameObject* owner) { m_owner = owner; }
    GameObject* GetOwner() const { return m_owner; }

protected:
    GameObject* m_owner = nullptr; //
};