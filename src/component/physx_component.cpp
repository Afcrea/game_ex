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
    PxMaterial* material = Physics::GetDefaultMaterial();

    if (m_isDynamic) {
        m_actor = PxCreateDynamic(*Physics::GetSDK(), physxTransform, geometry, *material, m_mass);
    } else {
        m_actor = PxCreateStatic(*Physics::GetSDK(), physxTransform, geometry, *material);
    }

    Physics::GetScene()->addActor(*m_actor);
}

void PhysXComponent::Update(float dt) {
    if (!m_actor) return;

    // if (m_isDynamic) {
    //     auto pos = m_owner->GetComponent<TransformComponent>()->GetPosition();
    //     PxVec3 velocity(pos.x, pos.y, pos.z);
    //     SPDLOG_INFO("PhysXComponent::Update() - Actor: {} Velocity: ({}, {}, {})", m_actor->getConcreteTypeName(), velocity.x, velocity.y, velocity.z);
    //     dynamicActor->addForce(velocity);
    // }
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