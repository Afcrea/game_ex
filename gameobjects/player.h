#pragma once
#include "common.h"
#include "gameobject.h"
#include "shader.h"
#include "buffer.h"
#include "vertex_layout.h"

CLASS_PTR(Player)
class Player : public GameObject{
public:
    ~Player();
    PlayerUPtr static Create();
    void Init() override;
    void Update(float dt) override;
    void Render(CameraPtr camera) override;
    void Shutdown() override;

private:
    Player() {};

    ShaderUPtr fs;
    ShaderUPtr vs;
};