#include "linerenderer_component.h"

#include "gameobject.h"

LineRendererComponent::~LineRendererComponent() {
}

LineRendererComponentPtr LineRendererComponent::Create() {
    auto lineRenderer = LineRendererComponentPtr(new LineRendererComponent());
    lineRenderer->Init();
    return std::move(lineRenderer);
}

void LineRendererComponent::Init() {
    m_program = Program::Create("Resource/line.vs", "Resource/line.fs");

    m_vertexLayout = VertexLayout::Create();
    m_vertexBuffer = Buffer::CreateWithData(GL_ARRAY_BUFFER, GL_DYNAMIC_DRAW, nullptr, sizeof(glm::vec3), 0);

    m_vertexLayout->SetAttrib(0, 3, GL_FLOAT, false, sizeof(glm::vec3), 0);
}

void LineRendererComponent::Update(float dt) {
    //if (!m_NeedsUpdate) return;

    m_vertexBuffer->Update(GL_ARRAY_BUFFER, GL_DYNAMIC_DRAW, m_LineVertices.data(), sizeof(glm::vec3), m_LineVertices.size());
    m_NeedsUpdate = false;
}

void LineRendererComponent::Shutdown() {
}

void LineRendererComponent::Render(CameraPtr camera) {
    if (m_LineVertices.empty()) return;

    glm::mat4 transform = camera->GetProjectionMatrix() * camera->GetViewMatrix();
    m_program->Use();

    glDisable(GL_DEPTH_TEST);
    glDepthMask(GL_FALSE);

    GLint locP = glGetUniformLocation(m_program->Get(), "uTransform");
    GLint locC = glGetUniformLocation(m_program->Get(), "uColor");
    glUniformMatrix4fv(locP, 1, GL_FALSE, glm::value_ptr(transform));
    glUniform4f(locC, 1.0f, 0.0f, 0.0f, 1.0f);

    m_vertexLayout->Bind();
    glDrawArrays(GL_LINES, 0, (GLsizei)m_LineVertices.size());

    glDepthMask(GL_TRUE);
    glEnable(GL_DEPTH_TEST);
}

void LineRendererComponent::SetLines(const std::vector<glm::vec3>& lines) {
    m_LineVertices = lines;
    m_NeedsUpdate = true;
}