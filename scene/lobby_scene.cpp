// MainMenuScene.cpp
#include "lobby_scene.h"
#include "ui_customize.h"
#include "scene_manager.h"
#include "main_scene.h"
#include "input.h"

void LobbyScene::Init() {
    // 초기화
    Ui_customize::Initialization();
}

void LobbyScene::Update(float deltaTime) {
    // if(Input::GetKey(eKeyCode::A)) {
    //     SPDLOG_INFO("A 눌림");
    // }
    // if(Input::GetMouseButton(eMouseButtonCode::Left)) {
    //     SPDLOG_INFO("마우스 왼쪽 버튼 눌림");
    // }
    // 게임 로직 업데이트
}

void LobbyScene::Render() {
    Ui_customize::MakeFrame();

    int windowWidth, windowHeight;
    glfwGetWindowSize(glfwGetCurrentContext(), &windowWidth, &windowHeight);

    buttonX = 200.0f;
    buttonY = 50.0f;
    buttonS = 50.0f;
    buttonPos = glm::vec2((windowWidth / 2.0f) - (buttonX / 2.0f), windowHeight / 2.0f);

    Ui_customize::MakeButton("Start", buttonX, buttonY, buttonS, buttonPos, StartGame);
    
    buttonPos.y = buttonPos.y + buttonS;

    Ui_customize::MakeButton("Option", buttonX, buttonY, buttonS, buttonPos, nullptr);

    buttonPos.y = buttonPos.y + buttonS;

    Ui_customize::MakeButton("Quit", buttonX, buttonY, buttonS, buttonPos, EndGame);
    
    Ui_customize::EndFrame();
}

void LobbyScene::Shutdown() {
    // 리소스 해제
}

void LobbyScene::StartGame() {
    SceneManager::SetScene(std::make_unique<MainScene>());
}

void LobbyScene::EndGame() {
    glfwSetWindowShouldClose(glfwGetCurrentContext(), true);
}