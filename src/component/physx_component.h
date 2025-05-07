#pragma once
#include "common.h"
#include "component.h"
#include "../Physics.h"
#include <PxPhysicsAPI.h>

CLASS_PTR(PhysXComponent)
class PhysXComponent : public Component {
public:
    static PhysXComponentPtr Create();
    ~PhysXComponent();
    void Configure(bool isDynamic, float mass = 1.0f);

    void Init() override;
    void Update(float deltaTime) override;
    void Shutdown() override;

    PxRigidActor* GetActor() const { return m_actor; }
    PxRigidDynamic* GetDynamicActor() const;
private:
    bool m_isDynamic;
    float m_mass;
    PxRigidActor* m_actor = nullptr;
};
    