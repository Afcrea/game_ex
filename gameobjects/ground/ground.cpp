#include "ground.h"
#include "component/renderer_component.h"
#include "component/transform_component.h"
#include "component/player_component.h"
#include "component/physx_component.h"
#include "component/linerenderer_component.h"

GroundUPtr Ground::Create() {
    auto ground = GroundUPtr(new Ground());
    ground->Init();
    return std::move(ground);
}

Ground::~Ground() {
    Shutdown();
}

void Ground::Init() {
    SetName("Ground");

    fs = Shader::CreateFromFile("Resource/lighting.fs", GL_FRAGMENT_SHADER);
    vs = Shader::CreateFromFile("Resource/lighting.vs", GL_VERTEX_SHADER);

    auto renderer = AddComponent<RendererComponent>();
    renderer->Configure("Resource/groundModel/floor.fbx");
    renderer->Configure(std::move(fs), std::move(vs));
    auto transform = AddComponent<TransformComponent>();
    transform->SetPosition(glm::vec3(0.0f, 0.0f, 0.0f));
    transform->SetScale(glm::vec3(100.0f, 1.0f, 100.0f));
    auto physics = AddComponent<PhysXComponent>();
    physics->Configure(false, 10.0f, 0.02f);
    physics->Configure(1.0f, 1.0f, 0.0f);
    auto lineRenderer = AddComponent<LineRendererComponent>();

    for (const auto& [type, component] : m_components) {
        if (component) {
            component->Init();
        }
    }
}

void Ground::Update(float dt) {
    for (const auto& [type, component] : m_components) {
        if (component) {
            component->Update(dt);
        }
    }
}

void Ground::Render(CameraPtr camera) {
    for (const auto& [type, component] : m_components) {
        if (component) {
            component->Render(camera);
        }
    }
}

void Ground::Shutdown() {
    SPDLOG_INFO("Ground shutdown");

    for (const auto& [type, component] : m_components) {
        if (component) {
            component->Shutdown();
        }
    }
    m_components.clear();
}
