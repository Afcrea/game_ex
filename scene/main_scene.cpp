#pragma once
#include "main_scene.h"
#include "player.h"
#include "Box.h"

void MainScene::Init() {
    // 초기화
    auto player = Player::Create();
    playerShaderProgram = Program::Create({player->GetFragmentShader(), player->GetVertexShader()});
    player->SetProgramID(playerShaderProgram->Get()); 
    player->Configure();

    gameObjects.push_back(GameObjectPtr(std::move(player)));

    auto box = Box::Create();
    boxShaderProgram = Program::Create({box->GetFragmentShader(), box->GetVertexShader()});
    box->SetProgramID(boxShaderProgram->Get());
    box->Configure();
    gameObjects.push_back(GameObjectPtr(std::move(box)));
}

void MainScene::Update(double deltaTime) {
    // 게임 로직 업데이트
}

void MainScene::Render() {
    glClearColor(0.1f, 0.1f, 0.15f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    for(auto gameObject : gameObjects) {
        gameObject->Render();
    }

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

    auto projection = glm::perspective(glm::radians(45.0f), (float)m_width / (float)m_height, 0.01f, 200.0f);
    
    glm::vec3 position = glm::vec3(1.0f, 1.0f, 10.0f);
    glm::vec3 position2 = glm::vec3(1.0f, -5.0f, 10.0f);

    // auto view = glm::lookAt(
    //     m_cameraPos,
    //     m_cameraPos + m_cameraFront,
    //     m_cameraUp);   

    glm::vec3 targetPos = position; // 모델 중심 (오브젝트 위치)
    float distance = 100.0f;                 // 모델에서 떨어진 거리
    float height = 2.0f;                   // 위쪽 높이
    float yaw = glm::radians(45.0f);       // 모델 뒤쪽에서 45도 비스듬히

    // 방향 계산 (XZ 평면 회전)
    glm::vec3 offset;
    offset.x = sin(yaw) * distance;
    offset.z = cos(yaw) * distance;
    offset.y = height;

    cameraPos = targetPos + offset;
    cameraTarget = targetPos;
    cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

    // View 행렬
    glm::mat4 view = glm::lookAt(cameraPos, cameraTarget, cameraUp);

    auto model = glm::mat4(1.0f);

    model = glm::translate(model, position);

    auto MVP = projection * view * model;

    model = glm::translate(model, position2);
    auto MVP2 = projection * view * model;

    glUseProgram(boxShaderProgram->Get());
    GLuint location = glGetUniformLocation(boxShaderProgram->Get(), "MVP");
    glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(MVP2));

    glUseProgram(playerShaderProgram->Get());
    GLuint location1 = glGetUniformLocation(playerShaderProgram->Get(), "MVP");
    glUniformMatrix4fv(location1, 1, GL_FALSE, glm::value_ptr(MVP));
}

void MainScene::Shutdown() {
    // 리소스 해제
    for(auto gameObject : gameObjects) {
        gameObject->Shutdown();
    }
}