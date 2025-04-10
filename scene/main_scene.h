#pragma once
#include "Scene.h"

class MainScene : public Scene {
public:
    void Init() override;
    void Update(float deltaTime) override;
    void Render() override;
    void Shutdown() override;
};