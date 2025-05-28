#include "backpack.h"
#include "component/renderer_component.h"
#include "component/transform_component.h"
#include "component/physx_component.h"
#include "component/linerenderer_component.h"
#include "component/backpack_component.h"

BackpackPtr Backpack::Create(glm::vec3 position, ModelPtr model, ProgramPtr program) {
    auto backpack = BackpackPtr(new Backpack());
    backpack->Init(position, model, program);

    return move(backpack);
}

void Backpack::Init(glm::vec3 position, ModelPtr model, ProgramPtr program) {
    SetName("Backpack");

    fs = Shader::CreateFromFile("Resource/lighting.fs", GL_FRAGMENT_SHADER);
    vs = Shader::CreateFromFile("Resource/lighting.vs", GL_VERTEX_SHADER);

    auto renderer = AddComponent<RendererComponent>(model, program);
    // renderer->Configure("Resource/backpackModel/backpack.obj");
    // renderer->Configure(std::move(fs), std::move(vs));
    auto transform = AddComponent<TransformComponent>();
    transform->SetPosition(position);
    transform->SetScale(glm::vec3(1.0f, 1.0f, 1.0f));
    auto physics = AddComponent<PhysXComponent>();
    physics->Configure(false, 1.0f, 1.0f);
    physics->SetTrigger(true);
    physics->SetHalfSize(transform->GetScale().x, transform->GetScale().y, transform->GetScale().z);
    #if defined(DEBUG)
        auto lineRenderer = AddComponent<LineRendererComponent>();
    #endif
    auto backpack = AddComponent<BackPackComponent>();

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