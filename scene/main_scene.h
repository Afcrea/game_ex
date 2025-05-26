#pragma once
#include "common.h"
#include "Scene.h"
#include "program.h"
#include "buffer.h"
#include "vertex_layout.h"
#include "camera.h"

CLASS_PTR(MainScene)
class MainScene : public Scene {
public:
    void Init() override;
    void Update(float deltaTime) override;
    void Render() override;
    void Shutdown() override;

    ProgramUPtr playerShaderProgram;
    ProgramUPtr boxShaderProgram;
    
private:
    bool m_cameraControl { false };
    glm::vec2 m_prevMousePos { glm::vec2(0.0f) };

    int m_width {640};
    int m_height {480};

    CameraPtr m_camera;

    bool m_paused = false;

    Light m_light {
        glm::vec3(0.0f, 100.0f, 0.0f),
        glm::vec3(0.0f, -90.0f, 180.0f),
        glm::vec2(20.0f, 5.0f),
        32.0f,
        glm::vec3(0.1f, 0.1f, 0.1f),
        glm::vec3(0.5f, 0.5f, 0.5f),
        glm::vec3(1.0f, 1.0f, 1.0f)
    };
};