#include "backpack.h"
#include "component/renderer_component.h"
#include "component/transform_component.h"
#include "component/physx_component.h"

Backpack::~Backpack() {
    Shutdown();
}

BackpackUPtr Backpack::Create() {
    auto backpack = BackpackUPtr(new Backpack());
    backpack->Init();

    return move(backpack);
}

void Backpack::Init() {
    fs = Shader::CreateFromFile("Resource/lighting.fs", GL_FRAGMENT_SHADER);
    vs = Shader::CreateFromFile("Resource/lighting.vs", GL_VERTEX_SHADER);

    auto renderer = AddComponent<RendererComponent>();
    renderer->Configure("Resource/backpackModel/backpack.obj");
    renderer->Configure(std::move(fs), std::move(vs));
    auto transform = AddComponent<TransformComponent>();
    transform->SetPosition(glm::vec3(2.0f, 5.0f, 2.0f));
    transform->SetScale(glm::vec3(2.0f, 2.1f, 2.1f));
    auto physics = AddComponent<PhysXComponent>();
    physics->Configure(true, 1.0f);

    for (const auto& [type, component] : m_components) {
        if (component) {
            component->Init();
        }
    }
}

void Backpack::Update(float dt) { 
    for (const auto& [type, component] : m_components) {
        if (component) {
            component->Update(dt);
        }
    }
}

void Backpack::Render(CameraPtr camera) {
    for (const auto& [type, component] : m_components) {
        if (component) {
            component->Render(camera);
        }
    }
}

void Backpack::Shutdown() {
    SPDLOG_INFO("Backpack shutdown");

    for (const auto& [type, component] : m_components) {
        if (component) {
            component->Shutdown();
        }
    }
    m_components.clear();
}