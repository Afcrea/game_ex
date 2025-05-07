#include "player.h"
#include "scene_manager.h"
#include "main_scene.h"
#include "component/renderer_component.h"
#include "component/transform_component.h"

Player::~Player() {
    SPDLOG_INFO("player shutdown");
}

PlayerUPtr Player::Create() {
    auto player = PlayerUPtr(new Player());
    player->Init();

    return move(player);
}

void Player::Configure() {
    
}

void Player::Init() {
    fs = Shader::CreateFromFile("gameobjects/lighting.fs", GL_FRAGMENT_SHADER);
    vs = Shader::CreateFromFile("gameobjects/lighting.vs", GL_VERTEX_SHADER);

    auto renderer = AddComponent<RendererComponent>();
    renderer->Configure("gameobjects/player.obj");
    renderer->Configure(std::move(fs), std::move(vs));
    auto transform = AddComponent<TransformComponent>();
    transform->SetPosition(glm::vec3(-2.0f, 1.0f, 0.0f));
}
void Player::Update(double dt) { 

}
void Player::Render() {
    
}
void Player::Shutdown() {

}