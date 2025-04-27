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
    fs = Shader::CreateFromFile("gameobjects/fragment.fs", GL_FRAGMENT_SHADER);
    vs = Shader::CreateFromFile("gameobjects/vertex.vs", GL_VERTEX_SHADER);

    auto renderer = AddComponent<RendererComponent>();

    auto transform = AddComponent<TransformComponent>();
}
void Player::Update(double dt) { 

}
void Player::Render() {
    
}
void Player::Shutdown() {

}