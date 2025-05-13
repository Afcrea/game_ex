#include "renderer_component.h"
#include "transform_component.h"
#include "animator_component.h"
#include "gameobject.h"

RendererComponent::~RendererComponent() {
}

RendererComponentUPtr RendererComponent::Create() {
    auto renderer = RendererComponentUPtr(new RendererComponent());
    return std::move(renderer);
}
// 파라미터에 모델 경로만 넘겨주고 플레이어에서 실행
void RendererComponent::Configure(const std::string& filename) {
    m_model =  Model::Load(filename);
}

void RendererComponent::Configure(ShaderPtr fs, ShaderPtr vs) {
    m_program = Program::Create({fs, vs});
}

// void RendererComponent::Render(CameraPtr camera) const {
    
//     auto projection = camera->GetProjectionMatrix();
       
//     glm::vec3 position = m_owner->GetComponent<TransformComponent>()->GetPosition();

//     auto model = glm::mat4(1.0f);

//     model = glm::translate(model, position);

//     //camera->SetTarget(position);

//     camera->SetView();

//     glm::mat4 view = camera->GetViewMatrix();

//     auto MVP = projection * view * model;

//     glUseProgram(m_program->Get());
//     GLuint location1 = glGetUniformLocation(m_program->Get(), "MVP");
//     glUniformMatrix4fv(location1, 1, GL_FALSE, glm::value_ptr(MVP));

//     m_model->Draw(m_program.get());
// }

void RendererComponent::Render(CameraPtr camera) {
    auto projection = camera->GetProjectionMatrix();
    auto view = camera->GetViewMatrix();
    glm::vec3 viewPos = camera->GetPosition();

    glm::vec3 position = m_owner->GetComponent<TransformComponent>()->GetPosition();
    glm::vec3 scale = m_owner->GetComponent<TransformComponent>()->GetScale();

    glm::vec3 rotation = m_owner->GetComponent<TransformComponent>()->GetRotation();

    glm::mat4 model = glm::translate(glm::mat4(1.0f), position);
    model = glm::scale(model, scale);

    glm::mat4 transform = projection * view * model;

    glUseProgram(m_program->Get());

    // MVP와 model matrix 전달
    GLint locTransform = glGetUniformLocation(m_program->Get(), "transform");
    glUniformMatrix4fv(locTransform, 1, GL_FALSE, glm::value_ptr(transform));

    GLint locModelTransform = glGetUniformLocation(m_program->Get(), "modelTransform");
    glUniformMatrix4fv(locModelTransform, 1, GL_FALSE, glm::value_ptr(model));

    // 카메라 위치 전달
    GLint locViewPos = glGetUniformLocation(m_program->Get(), "viewPos");
    glUniform3fv(locViewPos, 1, glm::value_ptr(viewPos));

    // 라이트 구조체 전달
    struct Light {
        glm::vec3 position { glm::vec3(2.0f, 2.0f, 2.0f) };
        glm::vec3 direction { glm::vec3(-1.0f, -1.0f, -1.0f) };
        glm::vec2 cutoff { glm::vec2(20.0f, 5.0f) };
        float distance { 32.0f };
        glm::vec3 ambient { glm::vec3(0.1f, 0.1f, 0.1f) };
        glm::vec3 diffuse { glm::vec3(0.5f, 0.5f, 0.5f) };
        glm::vec3 specular { glm::vec3(1.0f, 1.0f, 1.0f) };
    };
    Light light;

    m_program->SetUniform("light.position", light.position);
    m_program->SetUniform("light.direction", light.direction);
    m_program->SetUniform("light.cutoff", glm::vec2(
    cosf(glm::radians(light.cutoff[0])),
    cosf(glm::radians(light.cutoff[0] + light.cutoff[1]))));
    m_program->SetUniform("light.attenuation", GetAttenuationCoeff(light.distance));
    m_program->SetUniform("light.ambient", light.ambient);
    m_program->SetUniform("light.diffuse", light.diffuse);
    m_program->SetUniform("light.specular", light.specular);

    camera->SetView();

    auto animator = m_owner->GetComponent<AnimatorComponent>();
    if (animator) {
        auto boneMatrices = animator->GetBoneMatrices();
        GLint locBoneMatrices = glGetUniformLocation(m_program->Get(), "finalBonesMatrices");
        glUniformMatrix4fv(locBoneMatrices, static_cast<GLsizei>(boneMatrices.size()), GL_FALSE, glm::value_ptr(boneMatrices[0]));
    }
    
    m_model->Draw(m_program.get());
}
