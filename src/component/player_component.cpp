#include "player_component.h"
#include "transform_component.h"
#include "physx_component.h"
#include "input.h"
#include "gameobject.h"

PlayerComponent::~PlayerComponent() {
}

PlayerComponentUPtr PlayerComponent::Create() {
    auto player = PlayerComponentUPtr(new PlayerComponent());
    return std::move(player);
}

void PlayerComponent::Update(float dt) {
    auto physx = m_owner->GetComponent<PhysXComponent>();
    if (!physx) return;

    physx::PxRigidDynamic* actor = static_cast<physx::PxRigidDynamic*>(physx->GetActor());
    if (!actor) return;

    PxVec3 currentVelocity = actor->getLinearVelocity();

    PxVec3 desiredVelocity = currentVelocity; // 기본적으로 유지
    const float moveSpeed = 5.0f;

    if (Input::GetKey(eKeyCode::W)) {
        desiredVelocity.x = moveSpeed;
    }
    if (Input::GetKey(eKeyCode::S)) {
        desiredVelocity.x = -moveSpeed;
    }
    if (Input::GetKey(eKeyCode::A)) {
        desiredVelocity.z = -moveSpeed;
    }
    if (Input::GetKey(eKeyCode::D)) {
        desiredVelocity.z = moveSpeed;
    }

    // y속도는 중력에 의해 계속 변화해야 하므로 유지
    desiredVelocity.y = currentVelocity.y;

    actor->setLinearVelocity(desiredVelocity);

    auto transform = m_owner->GetComponent<TransformComponent>();
    if (Input::GetKey(eKeyCode::Q)) {
        transform->SetRotation(transform->GetRotation() + glm::vec3(0.0f, 1.0f, 0.0f));
    }
    //SPDLOG_INFO("PlayerComponent::Update: position: {}", transform->GetPosition());
}

void PlayerComponent::Render(CameraPtr camera) {
    auto transform = GetOwner()->GetComponent<TransformComponent>();
    camera->SetTarget(transform->GetPosition());
}