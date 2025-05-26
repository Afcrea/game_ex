#pragma once
#include "main_scene.h"
#include "player/player.h"
#include "backpack/backpack.h"
#include "ground/ground.h"

#include "component/renderer_component.h"
#include "component/transform_component.h"
#include "component/linerenderer_component.h"
#include "component/physx_component.h"
#include "input.h"
#include "physics.h"
#include "ui_customize.h"
#include "scene_manager.h"

void MainScene::Init() {
    Physics::Initialize();

    #if defined(NDEBUG)
        glfwSetInputMode(glfwGetCurrentContext(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    #endif
    // 초기화
    RequestSpawn<Ground>();
    RequestSpawn<Player>();
    RequestSpawn<Backpack>();
    RequestSpawn<Backpack>();

    m_camera = Camera::Create();
    m_camera->Configure(45.0f, (float)m_width / (float)m_height, 0.1f, 5000.0f, glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(0.0f, 1.0f, 0.0f));

    // OpenGL 상태 초기화
    glDisable(GL_BLEND);
    glDisable(GL_CULL_FACE);
    glDisable(GL_DEPTH_TEST);

    glEnable(GL_BLEND);
    glEnable(GL_CULL_FACE);
    glEnable(GL_DEPTH_TEST);
}

void MainScene::Update(float deltaTime) {

    if (Input::GetKeyDown(eKeyCode::Escape)) {
        m_paused = !m_paused;
        // ImGui cursor 모드 전환
        glfwSetInputMode(glfwGetCurrentContext(), GLFW_CURSOR, m_paused ? GLFW_CURSOR_NORMAL : GLFW_CURSOR_DISABLED);
    }
    if (m_paused) return;

    for (auto& req : m_pendingSpawn) {
        req.fn();  
    }
    m_pendingSpawn.clear();

    Physics::Simulation(deltaTime);
    Physics::FetchResults();

    // 게임 로직 업데이트
    for(auto gameObject : m_objects) {
        gameObject->Update(deltaTime);
    }

    if (!m_pendingRemove.empty()) {
        for (auto& rem : m_pendingRemove) {
            rem->Shutdown();  
            m_objects.erase(std::remove(m_objects.begin(), m_objects.end(), rem), m_objects.end());
        }
        m_pendingRemove.clear();
    }
}

void MainScene::Render() {
    glfwGetWindowSize(glfwGetCurrentContext(), &m_width, &m_height);
    m_camera->SetAspect((float)m_width / (float)m_height);

    glClearColor(0.1f, 0.1f, 0.15f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    m_camera->SetView();
    m_camera->SetLight(m_light);
    for(auto gameObject : m_objects) {
        gameObject->Render(m_camera);
    }
    
    if (m_paused) {
        // ImGui 페이즈
        Ui_customize::MakeFrame();
        // 화면 중앙에 두 버튼
        ImVec2 sz(200, 50), pos{ 
            (ImGui::GetIO().DisplaySize.x - sz.x)/2,
            (ImGui::GetIO().DisplaySize.y - sz.y)/2
        };
        Ui_customize::MakeButton("Resume", sz.x, sz.y, 10, {pos.x, pos.y}, 
            [&] {
                m_paused = false; 
            }
        );
        pos.y += sz.y + 10;
        Ui_customize::MakeButton("Back to Lobby", sz.x, sz.y, 10, {pos.x, pos.y}, 
            [&] {
                SceneManager::SetScene(std::make_unique<LobbyScene>());
            }
        );
        Ui_customize::EndFrame();
    }
    else {
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
        if(ImGui::Begin("Editor")) {
            // light sttings
            ImGui::Separator();
            ImGui::Text("Light Settings");
            ImGui::DragFloat3("Light Position", glm::value_ptr(m_light.position), 1.0f);
            ImGui::DragFloat3("Light Direction", glm::value_ptr(m_light.direction), 1.0f);
            ImGui::DragFloat2("Light Cutoff", glm::value_ptr(m_light.cutoff), 1.0f);
            ImGui::DragFloat("Light Distance", &m_light.distance, 1.0f);
            ImGui::ColorEdit3("Light Ambient", glm::value_ptr(m_light.ambient));
            ImGui::ColorEdit3("Light Diffuse", glm::value_ptr(m_light.diffuse));
            ImGui::ColorEdit3("Light Specular", glm::value_ptr(m_light.specular));
        }
        ImGui::End();
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    }
}

void MainScene::Shutdown() {
    // 리소스 해제
    for(auto gameObject : m_objects) {
        gameObject->Shutdown();
    }
    Physics::Shutdown();
}