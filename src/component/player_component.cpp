#include "player_component.h"
#include "transform_component.h"
#include "physx_component.h"
#include "animator_component.h"
#include "input.h"
#include "gameobject.h"
#include "Scene.h"
#include "backpack/backpack.h"

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

    PxVec3 desiredVelocity = PxVec3(0.0f, 0.0f, 0.0f); // 기본적으로 유지
    float moveSpeed = 5.0f;

    if(Input::GetKey(eKeyCode::ShiftLeft)) {
        moveSpeed = 20.0f;
    }

    if (Input::GetKey(eKeyCode::W)) {
        desiredVelocity.z = moveSpeed;
    }
    if (Input::GetKey(eKeyCode::S)) {
        desiredVelocity.z = -moveSpeed;
    }
    if (Input::GetKey(eKeyCode::A)) {
        desiredVelocity.x = moveSpeed;
    }
    if (Input::GetKey(eKeyCode::D)) {
        desiredVelocity.x = -moveSpeed;
    }

    // y속도는 중력에 의해 계속 변화해야 하므로 유지
    desiredVelocity.y = currentVelocity.y;

    actor->setLinearVelocity(desiredVelocity);

    auto transform = m_owner->GetComponent<TransformComponent>();

    if((desiredVelocity.x != 0.0f || desiredVelocity.z != 0.0f) && moveSpeed < 10.0f) {
        auto animator = m_owner->GetComponent<AnimatorComponent>();
        animator->SetCurrentAnimation(2);
    }
    else if((desiredVelocity.x != 0.0f || desiredVelocity.z != 0.0f) && moveSpeed > 10.0f) {
        auto animator = m_owner->GetComponent<AnimatorComponent>();
        animator->SetCurrentAnimation(1);
    }
    else {
        auto animator = m_owner->GetComponent<AnimatorComponent>();
        animator->SetCurrentAnimation(0);
    }

    if(Input::GetMouseButtonDown(eMouseButtonCode::Left)) {
        SPDLOG_INFO("누름");
        GetOwner()->GetScene()->RequestSpawn<Backpack>();
    }
}

void PlayerComponent::OnTriggerEnter(GameObject* other) {
    if (other->GetName() == "Backpack") {
        SPDLOG_INFO("PlayerComponent::OnTriggerEnter: Coin collected!");
        m_owner->GetScene()->Destroy(other);
    }
}

void PlayerComponent::Render(CameraPtr camera) {

    auto transform = GetOwner()->GetComponent<TransformComponent>();

    auto targetPos = transform->GetPosition();

    glm::mat4 model = transform->GetTransformMatrix();

    glm::vec4 localPos = glm::vec4(targetPos, 1.0f);
    glm::vec4 worldPos = model * localPos;

    glm::vec3 playerPos = targetPos;
    glm::vec3 forward   = transform->GetForward();

    camera->Follow(
        glm::vec3(worldPos.x, worldPos.y, worldPos.z),
        forward,
        50.0f,
        30.0f
    );
}