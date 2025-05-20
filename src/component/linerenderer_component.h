#pragma once

#include "component.h"
#include "program.h"
#include "buffer.h"
#include "vertex_layout.h"

CLASS_PTR(LineRendererComponent);
class LineRendererComponent : public Component {
public:
    static LineRendererComponentPtr Create();
    ~LineRendererComponent() override;

    void Init() override;
    void Update(float dt) override;
    void Shutdown() override;
    void Render(CameraPtr camera);   

    void SetLines(const std::vector<glm::vec3>& lines);

private:
    ProgramUPtr m_program;
    VertexLayoutUPtr m_vertexLayout;
    BufferPtr m_vertexBuffer;

    std::vector<glm::vec3> m_LineVertices;
    bool m_NeedsUpdate = false;
};