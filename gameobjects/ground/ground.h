#pragma once

#include "common.h"
#include "gameobject.h"
#include "shader.h"
#include "buffer.h"
#include "vertex_layout.h"

CLASS_PTR(Ground)
class Ground : public GameObject {
public:
    static GroundPtr Create();
    ~Ground() = default;

    void Init() override;
    void Update(float dt) override;
    void Render(CameraPtr camera) override;
    void Shutdown() override;

private:
    Ground() {};

    ShaderUPtr fs;
    ShaderUPtr vs;
};