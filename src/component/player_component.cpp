#include "player_component.h"
#include "transform_component.h"
#include "input.h"
#include "gameobject.h"

PlayerComponent::~PlayerComponent() {
}

PlayerComponentUPtr PlayerComponent::Create() {
    auto player = PlayerComponentUPtr(new PlayerComponent());
    return std::move(player);
}

void PlayerComponent::Update(float dt) {
    auto transform = GetOwner()->GetComponent<TransformComponent>();
    if (Input::GetKey(eKeyCode::W)) {
        transform->SetPosition(transform->GetPosition() + glm::vec3(1.0f, 0.0f, 0.0f) * 10.0f * dt);
    }
    if (Input::GetKey(eKeyCode::S)) {
        transform->SetPosition(transform->GetPosition() + glm::vec3(-1.0f, 0.0f, 0.0f) * 10.0f * dt);
    }
    if (Input::GetKey(eKeyCode::A)) {
        transform->SetPosition(transform->GetPosition() + glm::vec3(0.0f, 0.0f, -1.0f) * 10.0f * dt);
    }
    if (Input::GetKey(eKeyCode::D)) {
        transform->SetPosition(transform->GetPosition() + glm::vec3(0.0f, 0.0f, 1.0f) * 10.0f * dt);
    }
    if (Input::GetKey(eKeyCode::Space)) {
        transform->SetPosition(transform->GetPosition() + glm::vec3(0.0f, 1.0f, 0.0f) * 10.0f * dt);
    }
    if (Input::GetKey(eKeyCode::CtrlLeft)) {
        transform->SetPosition(transform->GetPosition() + glm::vec3(0.0f, -1.0f, 0.0f) * 10.0f * dt);
    }
}

void PlayerComponent::Render(CameraPtr camera) {
    auto transform = GetOwner()->GetComponent<TransformComponent>();
    camera->SetTarget(transform->GetPosition());
}