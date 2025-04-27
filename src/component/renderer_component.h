#pragma once
#include "component.h"
#include "buffer.h"
#include "vertex_layout.h"
#include "program.h"
#include "model.h"
#include "camera.h"

CLASS_PTR(RendererComponent)
class RendererComponent : public Component {
public:
    static RendererComponentUPtr Create();
    ~RendererComponent();
    
    void Configure(const std::string& filename);
    void Render(const Program* program, CameraPtr camera) const;
private:
    ModelUPtr m_model;
};
