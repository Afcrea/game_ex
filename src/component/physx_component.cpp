#include "physx_component.h"
#include "transform_component.h"
#include "gameobject.h"

PhysXComponent::~PhysXComponent() {
    Shutdown();
}

PhysXComponentPtr PhysXComponent::Create() {
    auto physx = PhysXComponentPtr(new PhysXComponent());
    return std::move(physx);
}

void PhysXComponent::Configure(bool isDynamic, float mass) { 
    m_isDynamic = isDynamic; 
    m_mass = mass; 
}

void PhysXComponent::Configure(float staticFriction, float dynamicFriction, float restitution) { 
    m_staticFriction = staticFriction; 
    m_dynamicFriction = dynamicFriction; 
    m_restitution = restitution;
}

PxRigidDynamic* PhysXComponent::GetDynamicActor() const {
    if (!m_isDynamic || !m_actor) return nullptr;
    return static_cast<PxRigidDynamic*>(m_actor);
}

void PhysXComponent::Init() {
    
    auto transform = m_owner->GetComponent<TransformComponent>();
    if (!transform) return;

    glm::vec3 pos = transform->GetPosition();
    glm::vec3 scale = transform->GetScale();
    PxBoxGeometry geometry(scale.x * 0.5f, scale.y * 0.5f, scale.z * 0.5f);
    
    PxTransform physxTransform(PxVec3(pos.x, pos.y, pos.z));
    PxMaterial* material = Physics::GetSDK()->createMaterial(m_staticFriction, m_dynamicFriction, m_restitution);

    if (m_isDynamic) {
        m_actor = PxCreateDynamic(*Physics::GetSDK(), physxTransform, geometry, *material, m_mass);
    } else {
        m_actor = PxCreateStatic(*Physics::GetSDK(), physxTransform, geometry, *material);
    }

    Physics::GetScene()->addActor(*m_actor);
}

void PhysXComponent::Update(float dt) {
    if (!m_actor) return;

    auto dynamicActor = GetDynamicActor();
    if (!dynamicActor) return;

    PxVec3 v = dynamicActor->getLinearVelocity();

    if (v.y > 0.0f) {
        v.y = 0.0f;  // 위로 튕기는 걸 제거
        dynamicActor->setLinearVelocity(v);
    }

    PxTransform pose = m_actor->getGlobalPose();
    auto transform = m_owner->GetComponent<TransformComponent>();
    if (transform) {
        transform->SetPosition(glm::vec3(pose.p.x, pose.p.y, pose.p.z));
    }
}

void PhysXComponent::Shutdown() {
    if (m_actor) {
        SPDLOG_INFO("PhysXComponent::Shutdown() - Destroying actor: {}", m_actor->getConcreteTypeName());
        m_actor->release();
        m_actor = nullptr;
    }
}