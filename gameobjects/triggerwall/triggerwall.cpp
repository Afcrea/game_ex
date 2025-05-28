#include "triggerwall.h"
#include "component/renderer_component.h"
#include "component/transform_component.h"
#include "component/physx_component.h"
#include "component/linerenderer_component.h"
#include "component/triggerwall_component.h"

TriggerWallPtr TriggerWall::Create(std::string name, glm::vec3 position, glm::vec3 rotation) {
    auto triggerwall = TriggerWallPtr(new TriggerWall());
    triggerwall->Init(name, position, rotation);

    return move(triggerwall);
}

void TriggerWall::Init(std::string name, glm::vec3 position, glm::vec3 rotation) {
    SetName(name);

    fs = Shader::CreateFromFile("Resource/lighting.fs", GL_FRAGMENT_SHADER);
    vs = Shader::CreateFromFile("Resource/lighting.vs", GL_VERTEX_SHADER);

    auto transform = AddComponent<TransformComponent>();
    transform->SetPosition(position);
    transform->SetRotation(rotation);
    transform->SetScale(glm::vec3(50.0f, 50.0f, 1.0f));
    auto physics = AddComponent<PhysXComponent>();
    physics->Configure(false, 1.0f, 1.0f);
    physics->SetTrigger(true);
    physics->Configure(1.0f, 1.0f, 0.0f);
    physics->SetHalfSize(transform->GetScale().x, transform->GetScale().y, transform->GetScale().z);
    #if defined(DEBUG)
        auto lineRenderer = AddComponent<LineRendererComponent>();
    #endif
    auto triggerwall = AddComponent<TriggerWallComponent>();

    for (const auto& [type, component] : m_components) {
        if (component) {
            component->Init();
        }
    }
}
void TriggerWall::Update(float dt) {
    for (const auto& [type, component] : m_components) {
        if (component) {
            component->Update(dt);
        }
    }
}
void TriggerWall::Render(CameraPtr camera) {
    for (const auto& [type, component] : m_components) {
        if (component) {
            component->Render(camera);
        }
    }
}
void TriggerWall::Shutdown() {    
    for (const auto& [type, component] : m_components) {
        if (component) {
            component->Shutdown();
        }
    }
    m_components.clear();
}