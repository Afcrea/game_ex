#pragma once
#include "common.h"
#include "gameobject.h"
#include "shader.h"
#include "buffer.h"
#include "vertex_layout.h"

CLASS_PTR(Backpack)
class Backpack : public GameObject{
public:
    ~Backpack();
    BackpackUPtr static Create();
    void Init() override;
    void Update(float dt) override;
    void Render(CameraPtr camera) override;
    void Shutdown() override;
private:
    Backpack() {};

    ShaderUPtr fs;
    ShaderUPtr vs;
};