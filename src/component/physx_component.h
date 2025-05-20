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

// PhysXComponent.h 또는 cpp 상단에 추가
struct IgnoreSelfFilter : public physx::PxQueryFilterCallback {
    physx::PxRigidActor* selfActor;
    IgnoreSelfFilter(physx::PxRigidActor* actor)
    : selfActor(actor) {}

    // 1) 사전 필터: 자기 자신은 eNONE로 무시, 나머지는 블록
    virtual physx::PxQueryHitType::Enum preFilter(
        const physx::PxFilterData& filterData,
        const physx::PxShape* shape,
        const physx::PxRigidActor* actor,
        physx::PxHitFlags& queryFlags) override
    {
        if (actor == selfActor)
            return physx::PxQueryHitType::eNONE;   // 이 액터 건너뛰기
        return physx::PxQueryHitType::eBLOCK;      // 첫 충돌 지점에서 멈춤
    }

    // 2) 후처리 필터: 기본적으로 블록
    virtual physx::PxQueryHitType::Enum postFilter(
        const physx::PxFilterData& filterData,
        const physx::PxQueryHit& hit,
        const physx::PxShape* shape,
        const physx::PxRigidActor* actor) override
    {
        return physx::PxQueryHitType::eBLOCK;
    }
};
