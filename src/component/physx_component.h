#pragma once
#include "common.h"
#include "component.h"
#include "../Physics.h"
#include <PxPhysicsAPI.h>
#include "extensions/PxRigidActorExt.h"

CLASS_PTR(PhysXComponent)
class PhysXComponent : public Component {
public:
    static PhysXComponentPtr Create();
    ~PhysXComponent();
    void Configure(bool isDynamic, float mass = 1.0f, float offset = 1.0f);
    void Configure(float staticFriction, float dynamicFriction, float restitution);
    void SetTrigger(bool isTrigger) { m_isTrigger = isTrigger; }

    void Init() override;
    void Update(float deltaTime) override;
    void Shutdown() override;

    PxRigidActor* GetActor() const { return m_actor; }
    PxRigidDynamic* GetDynamicActor() const;
    bool IsSphereOverlap(float radius);
    bool RaycastFront(float maxDistance, GameObject*& outHitObject);
private:
    bool m_isDynamic;
    bool m_isTrigger = false;
    float m_mass;
    float m_dynamicFriction = 0.5f;
    float m_staticFriction = 0.5f;
    float m_restitution = 0.5f;
    float m_offset = 1.0f;

    PxRigidActor* m_actor = nullptr;
};