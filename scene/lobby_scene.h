#pragma once
#include "Scene.h"
#include "common.h"

class LobbyScene : public Scene {
public:
    void Init() override;
    void Update(double deltaTime) override;
    void Render() override;
    void Shutdown() override;
    static void StartGame();
    static void EndGame();
private:
    float buttonX;
    float buttonY;
    float buttonS;
    glm::vec2 buttonPos;
};