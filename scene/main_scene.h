#pragma once
#include "common.h"
#include "gameobject.h"
#include "Scene.h"
#include "player.h"
#include "program.h"
#include "buffer.h"
#include "vertex_layout.h"
#include "camera.h"

class MainScene : public Scene {
public:
    void Init() override;
    void Update(float deltaTime) override;
    void Render() override;
    void Shutdown() override;

    ProgramUPtr playerShaderProgram;
    ProgramUPtr boxShaderProgram;
    
private:
    std::vector<std::shared_ptr<GameObject>> gameObjects;
    bool m_cameraControl { false };
    glm::vec2 m_prevMousePos { glm::vec2(0.0f) };

    int m_width {640};
    int m_height {480};

    CameraPtr m_camera;
};