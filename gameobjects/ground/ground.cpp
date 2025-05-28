#include "ground.h"
#include "component/renderer_component.h"
#include "component/transform_component.h"
#include "component/player_component.h"
#include "component/physx_component.h"
#include "component/linerenderer_component.h"

GroundPtr Ground::Create(std::string name, glm::vec3 position, glm::vec3 rotation) {
    auto ground = GroundPtr(new Ground());
    ground->Init(name, position, rotation);
    return std::move(ground);
}

void Ground::Init(std::string name, glm::vec3 position, glm::vec3 rotation) {
    SetName(name);

    fs = Shader::CreateFromFile("Resource/lighting.fs", GL_FRAGMENT_SHADER);
    vs = Shader::CreateFromFile("Resource/lighting.vs", GL_VERTEX_SHADER);

    auto renderer = AddComponent<RendererComponent>();
    renderer->Configure("Resource/groundModel/ground.fbx");
    renderer->Configure(std::move(fs), std::move(vs));
    auto transform = AddComponent<TransformComponent>();
    transform->SetPosition(position);
    transform->SetRotation(rotation);
    transform->SetScale(glm::vec3(50.0f, 1.0f, 50.0f));
    auto physics = AddComponent<PhysXComponent>();
    physics->Configure(false, 10.0f, 0.02f);
    physics->Configure(1.0f, 1.0f, 0.0f);
    physics->SetHalfSize(transform->GetScale().x, transform->GetScale().y, transform->GetScale().z);
    #if defined(DEBUG)
        auto lineRenderer = AddComponent<LineRendererComponent>();
    #endif

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
