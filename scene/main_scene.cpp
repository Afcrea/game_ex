#pragma once
#include "main_scene.h"
#include "player.h"
void MainScene::Init() {
    // 초기화
    auto player = Player::Create();
    
    gameObjects.push_back(std::shared_ptr<GameObject>(std::move(player)));
}

void MainScene::Update(double deltaTime) {
    // 게임 로직 업데이트
}

void MainScene::Render() {

    float angle = static_cast<float>(glfwGetTime()) * glm::pi<float>() * 0.5f;
    auto x = sinf(angle) * 10.0f;
    auto z = cosf(angle) * 10.0f;
    auto cameraPos = glm::vec3(x, 0.0f, z);
    auto cameraTarget = glm::vec3(0.0f, 0.0f, 0.0f);
    auto cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

	m_cameraFront =
        glm::rotate(glm::mat4(1.0f),
            glm::radians(m_cameraYaw), glm::vec3(0.0f, 1.0f, 0.0f)) *
        glm::rotate(glm::mat4(1.0f),
            glm::radians(m_cameraPitch), glm::vec3(1.0f, 0.0f, 0.0f)) *
        glm::vec4(0.0f, 0.0f, -1.0f, 0.0f);

    auto projection = glm::perspective(glm::radians(45.0f), (float)m_width / (float)m_height, 0.01f, 20.0f);
    
    auto view = glm::lookAt(
        m_cameraPos,
        m_cameraPos + m_cameraFront,
        m_cameraUp);   

    auto model = glm::mat4(1.0f);

    auto MVP = projection * view * model;

    GLint currentProgram = 0;
    glGetIntegerv(GL_CURRENT_PROGRAM, &currentProgram);

    GLuint location = glGetUniformLocation(currentProgram, "MVP");
    glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(MVP));

    m_cameraPos -= 0.001f * m_cameraFront;

    m_cameraPitch = 45;

    SPDLOG_INFO("{}", m_cameraPitch);

    for(auto gameObject : gameObjects) {
        gameObject->Render();
    }
}

void MainScene::Shutdown() {
    // 리소스 해제
}