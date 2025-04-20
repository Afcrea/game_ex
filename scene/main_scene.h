#pragma once
#include "common.h"
#include "gameobject.h"
#include "Scene.h"
#include "player.h"

class MainScene : public Scene {
public:
    void Init() override;
    void Update(double deltaTime) override;
    void Render() override;
    void Shutdown() override;

private:
    std::vector<std::shared_ptr<GameObject>> gameObjects;
    bool m_cameraControl { false };
    glm::vec2 m_prevMousePos { glm::vec2(0.0f) };

    int m_width {640};
    int m_height {480};

    float m_cameraPitch { 0.0f };
    float m_cameraYaw { 0.0f };
    glm::vec3 m_cameraPos { glm::vec3(0.0f, 0.0f, 3.0f) };
    glm::vec3 m_cameraFront { glm::vec3(0.0f, 0.0f, -1.0f) };
    glm::vec3 m_cameraUp { glm::vec3(0.0f, 1.0f, 0.0f) };
};