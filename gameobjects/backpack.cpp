#include "backpack.h"
#include "scene_manager.h"
#include "main_scene.h"
#include "component/renderer_component.h"
#include "component/transform_component.h"

Backpack::~Backpack() {
    SPDLOG_INFO("Backpack shutdown");
}

BackpackUPtr Backpack::Create() {
    auto backpack = BackpackUPtr(new Backpack());
    backpack->Init();

    return move(backpack);
}

void Backpack::Configure() {
    
}

void Backpack::Init() {
    fs = Shader::CreateFromFile("gameobjects/lighting.fs", GL_FRAGMENT_SHADER);
    vs = Shader::CreateFromFile("gameobjects/lighting.vs", GL_VERTEX_SHADER);

    auto renderer = AddComponent<RendererComponent>();
    renderer->Configure("gameobjects/backpack/backpack.obj");
    renderer->Configure(std::move(fs), std::move(vs));
    auto transform = AddComponent<TransformComponent>();
    transform->SetPosition(glm::vec3(2.0f, 0.0f, 2.0f));
}
void Backpack::Update(double dt) { 

}
void Backpack::Render() {
    
}
void Backpack::Shutdown() {

}