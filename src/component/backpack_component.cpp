#include "backpack_component.h"
#include "player_component.h"
#include "transform_component.h"
#include "physx_component.h"
#include "animator_component.h"
#include "input.h"
#include "gameobject.h"
#include "Scene.h"

BackPackComponent::~BackPackComponent() {
}

BackPackComponentUPtr BackPackComponent::Create() {
    auto backpack = BackPackComponentUPtr(new BackPackComponent());
    return std::move(backpack);
}

void BackPackComponent::Update(float dt) {
    auto physx = m_owner->GetComponent<PhysXComponent>();
    if (!physx) return;

    physx::PxRigidDynamic* actor = static_cast<physx::PxRigidDynamic*>(physx->GetActor());
    if (!actor) return;
    float moveSpeed = 5.0f;
    if (auto st = actor->is<PxRigidStatic>()) {
        auto transform = m_owner->GetComponent<TransformComponent>();
        if (!transform) return;

        glm::mat4 model = transform->GetTransformMatrix();
        glm::vec3 position = model[3];
        float moveSpeed = 5.0f;

        if((static_cast<int>(glfwGetTime()) % 20) >= 10.0f) {
            moveSpeed = -5.0f;
        }
        else {
            moveSpeed = 5.0f;
        }

        glm::vec3 newPos = position;

        newPos.z += moveSpeed * dt;

        PxTransform newPose(PxVec3(newPos.x, newPos.y, newPos.z),
                            st->getGlobalPose().q);
        // Static은 simulate() 뒤에도 유지되므로, 언제든 setGlobalPose OK
        st->setGlobalPose(newPose);
        transform->SetPosition(newPos);
    }
}

void BackPackComponent::OnTriggerEnter(GameObject* other) {
}