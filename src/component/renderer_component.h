#pragma once
#include "component.h"
#include "buffer.h"
#include "vertex_layout.h"
#include "shader.h"
#include "program.h"
#include "model.h"
#include "camera.h"

CLASS_PTR(RendererComponent)
class RendererComponent : public Component {
public:
    static RendererComponentPtr Create(ModelPtr model = nullptr, ProgramPtr program = nullptr);
    ~RendererComponent();
    void Init(ModelPtr model, ProgramPtr program);
    void Configure(const std::string& filename);
    void Configure(ShaderPtr fs, ShaderPtr vs);
    void Render(CameraPtr camera) override;

    ModelPtr GetModel() const { return m_model; }
private:
    
    ModelPtr m_model;
    ProgramPtr m_program;
};
