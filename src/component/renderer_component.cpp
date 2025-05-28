#include "renderer_component.h"
#include "transform_component.h"
#include "animator_component.h"
#include "gameobject.h"

RendererComponent::~RendererComponent() {
}

RendererComponentPtr RendererComponent::Create(ModelPtr model, ProgramPtr program) {
    auto renderer = RendererComponentPtr(new RendererComponent());
    if(model || program)
        renderer->Init(model, program);
    return std::move(renderer);
}

void RendererComponent::Init(ModelPtr model, ProgramPtr program) {
    m_model = std::move(model);
    m_program = std::move(program);
}

// 파라미터에 모델 경로만 넘겨주고 플레이어에서 실행
void RendererComponent::Configure(const std::string& filename) {
    m_model =  Model::Load(filename);
}

void RendererComponent::Configure(ShaderPtr fs, ShaderPtr vs) {
    m_program = Program::Create({fs, vs});
}

void RendererComponent::Render(CameraPtr camera) {
    if(m_model == nullptr || m_program == nullptr) {
        SPDLOG_ERROR("RendererComponent::Render - Model or Program is not configured.");
        return;
    }
    glEnable(GL_DEPTH_TEST);
    glDepthMask(GL_TRUE);

    glm::vec3 targetPos = camera->GetTarget();

    glm::vec3 position = m_owner->GetComponent<TransformComponent>()->GetPosition();

    auto projection = camera->GetProjectionMatrix();
    auto view = camera->GetViewMatrix();

    glm::vec3 viewPos = camera->GetPosition();
    
    glm::vec3 scale = m_owner->GetComponent<TransformComponent>()->GetScale();

    glm::vec3 rotation = m_owner->GetComponent<TransformComponent>()->GetRotation();
    
    glm::mat4 model = m_owner->GetComponent<TransformComponent>()->GetTransformMatrix();

    glm::mat4 transform = projection * view * model;

    glm::vec4 localPos = glm::vec4(position, 1.0f);
    glm::vec4 worldPos = model * localPos;

    glm::vec4 clipLocal = projection * view * localPos;
    glm::vec3 ndcLocal  = glm::vec3(clipLocal) / clipLocal.w;

    glm::vec4 clipWorld = projection * view * worldPos;
    glm::vec3 ndcWorld  = glm::vec3(clipWorld) / clipWorld.w;

    std::string name = m_owner->GetName();

    glUseProgram(m_program->Get());

    // MVP와 model matrix 전달
    GLint locTransform = glGetUniformLocation(m_program->Get(), "transform");
    glUniformMatrix4fv(locTransform, 1, GL_FALSE, glm::value_ptr(transform));

    GLint locModelTransform = glGetUniformLocation(m_program->Get(), "modelTransform");
    glUniformMatrix4fv(locModelTransform, 1, GL_FALSE, glm::value_ptr(model));

    // 카메라 위치 전달
    GLint locViewPos = glGetUniformLocation(m_program->Get(), "viewPos");
    glUniform3fv(locViewPos, 1, glm::value_ptr(viewPos));

    Light light = camera->GetLight();

    m_program->SetUniform("light.position", light.position);
    m_program->SetUniform("light.direction", light.direction);
    m_program->SetUniform("light.cutoff", glm::vec2(
    cosf(glm::radians(light.cutoff[0])),
    cosf(glm::radians(light.cutoff[0] + light.cutoff[1]))));
    m_program->SetUniform("light.attenuation", GetAttenuationCoeff(light.distance));
    m_program->SetUniform("light.ambient", light.ambient);
    m_program->SetUniform("light.diffuse", light.diffuse);
    m_program->SetUniform("light.specular", light.specular);

    auto animator = m_owner->GetComponent<AnimatorComponent>();
    if (animator) {
        auto boneMatrices = animator->GetBoneMatrices();
        GLint locBoneMatrices = glGetUniformLocation(m_program->Get(), "finalBonesMatrices");
        glUniformMatrix4fv(locBoneMatrices, static_cast<GLsizei>(boneMatrices.size()), GL_FALSE, glm::value_ptr(boneMatrices[0]));
    }
    
    m_model->Draw(m_program.get());
}
