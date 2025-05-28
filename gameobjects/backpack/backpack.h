#pragma once
#include "common.h"
#include "gameobject.h"
#include "shader.h"
#include "buffer.h"
#include "vertex_layout.h"
#include "program.h"
#include "model.h"

CLASS_PTR(Backpack)
class Backpack : public GameObject{
public:
    ~Backpack() = default;
    BackpackPtr static Create(glm::vec3 position, ModelPtr model = nullptr, ProgramPtr program = nullptr);
    void Init(glm::vec3 position, ModelPtr model, ProgramPtr program);
    void Update(float dt) override;
    void Render(CameraPtr camera) override;
    void Shutdown() override;
private:
    Backpack() {};

    ShaderUPtr fs;
    ShaderUPtr vs;
};