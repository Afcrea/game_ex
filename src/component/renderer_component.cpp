#include "renderer_component.h"
#include "transform_component.h"
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

void RendererComponent::Render(const Program* program, CameraPtr camera) const {
    
    auto projection = camera->GetProjectionMatrix();
       
    glm::vec3 position = m_owner->GetComponent<TransformComponent>()->GetPosition();

    auto model = glm::mat4(1.0f);

    model = glm::translate(model, position);

    //camera->SetTarget(position);

    camera->SetView();

    glm::mat4 view = camera->GetViewMatrix();

    auto MVP = projection * view * model;

    glUseProgram(program->Get());
    GLuint location1 = glGetUniformLocation(program->Get(), "MVP");
    glUniformMatrix4fv(location1, 1, GL_FALSE, glm::value_ptr(MVP));

    m_model->Draw(program);
}