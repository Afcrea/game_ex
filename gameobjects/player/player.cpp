#include "player.h"
#include "component/renderer_component.h"
#include "component/transform_component.h"
#include "component/player_component.h"
#include "component/physx_component.h"
#include "component/animator_component.h"

Player::~Player() {
    Shutdown();
}

PlayerUPtr Player::Create() {
    auto player = PlayerUPtr(new Player());
    player->Init();

    return move(player);
}

void Player::Init() {
    fs = Shader::CreateFromFile("gameobjects/lighting.fs", GL_FRAGMENT_SHADER);
    vs = Shader::CreateFromFile("gameobjects/bone.vs", GL_VERTEX_SHADER);

    auto renderer = AddComponent<RendererComponent>();
    renderer->Configure("gameobjects/player/playerModel/Y_Bot.fbx");
    renderer->Configure(std::move(fs), std::move(vs));
    auto transform = AddComponent<TransformComponent>();
    transform->SetPosition(glm::vec3(-2.0f, 4.0f, 0.0f));
    transform->SetScale(glm::vec3(0.1f, 0.1f, 0.1f));
    auto player = AddComponent<PlayerComponent>();
    auto physics = AddComponent<PhysXComponent>();
    physics->Configure(true, 10.0f);
    physics->Configure(1.0f, 1.0f, 0.0f);
    auto animator = AddComponent<AnimatorComponent>();
    animator->Configure("gameobjects/player/playerModel/Idle.fbx");

    for (const auto& [type, component] : m_components) {
        if (component) {
            component->Init();
        }
    }
}
void Player::Update(float dt) {
    for (const auto& [type, component] : m_components) {
        if (component) {
            component->Update(dt);
        }
    }
}
void Player::Render(CameraPtr camera) {
    for (const auto& [type, component] : m_components) {
        if (component) {
            component->Render(camera);
        }
    }
}
void Player::Shutdown() {    
    for (const auto& [type, component] : m_components) {
        if (component) {
            component->Shutdown();
        }
    }
    m_components.clear();
}