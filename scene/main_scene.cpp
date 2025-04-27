#pragma once
#include "main_scene.h"
#include "player.h"
#include "Box.h"
#include "input.h"
#include "component/renderer_component.h"

void MainScene::Init() {
    // 초기화
    auto player = Player::Create();
    playerShaderProgram = Program::Create({player->GetFragmentShader(), player->GetVertexShader()});
    player->SetProgramID(playerShaderProgram->Get());
    player->GetComponent<RendererComponent>()->Configure("gameobjects/player.obj");
    //player->Configure();

    gameObjects.push_back(GameObjectPtr(std::move(player)));

    auto box = Box::Create();
    boxShaderProgram = Program::Create({box->GetFragmentShader(), box->GetVertexShader()});
    box->SetProgramID(boxShaderProgram->Get());
    box->Configure();
    gameObjects.push_back(GameObjectPtr(std::move(box)));

    m_camera = Camera::Create();
    m_camera->Configure(45.0f, (float)m_width / (float)m_height, 0.1f, 100.0f, glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(0.0f, 1.0f, 0.0f));
}

void MainScene::Update(double deltaTime) {
    // 게임 로직 업데이트
    for(auto gameObject : gameObjects) {
        gameObject->Update(deltaTime);
    }

    if(Input::GetKey(eKeyCode::W)) {
        m_camera->Move(m_camera->GetFront() * 0.1f);
    }
    if(Input::GetKey(eKeyCode::S)) {
        m_camera->Move(-m_camera->GetFront() * 0.1f);
    }
    if(Input::GetKey(eKeyCode::A)) {
        m_camera->Move(-m_camera->GetRight() * 0.1f);
    }
    if(Input::GetKey(eKeyCode::D)) {
        m_camera->Move(m_camera->GetRight() * 0.1f);
    }
    if(Input::GetKey(eKeyCode::Space)) {
        m_camera->Move(glm::vec3(0.0f, 1.0f, 0.0f) * 0.1f);
    }
    if(Input::GetKey(eKeyCode::CtrlLeft)) {
        m_camera->Move(-glm::vec3(0.0f, 1.0f, 0.0f) * 0.1f);
    }

}

void MainScene::Render() {
    glClearColor(0.1f, 0.1f, 0.15f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    for(auto gameObject : gameObjects) {
        auto rendererComponent = gameObject->GetComponent<RendererComponent>();

        if(rendererComponent) {
            rendererComponent->Render(boxShaderProgram.get(), m_camera);
        }
    }
}

void MainScene::Shutdown() {
    // 리소스 해제
    for(auto gameObject : gameObjects) {
        gameObject->Shutdown();
    }
}