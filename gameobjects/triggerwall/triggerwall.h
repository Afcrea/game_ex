#pragma once

#include "common.h"
#include "gameobject.h"
#include "shader.h"
#include "buffer.h"
#include "vertex_layout.h"

CLASS_PTR(TriggerWall)
class TriggerWall : public GameObject{
public:
    ~TriggerWall() = default;
    TriggerWallPtr static Create(std::string name, glm::vec3 position, glm::vec3 rotation);
    void Init(std::string name, glm::vec3 position, glm::vec3 rotation);
    void Update(float dt) override;
    void Render(CameraPtr camera) override;
    void Shutdown() override;

private:
    TriggerWall() {};

    ShaderUPtr fs;
    ShaderUPtr vs;
};