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
    m_camera->Configure(45.0f, (float)m_width / (float)m_height, 0.1f, 1000.0f, glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(0.0f, 1.0f, 0.0f));

    // OpenGL 상태 초기화
    glDisable(GL_BLEND);
    glDisable(GL_CULL_FACE);
    glDisable(GL_DEPTH_TEST);

    glEnable(GL_BLEND);
    glEnable(GL_CULL_FACE);
    glEnable(GL_DEPTH_TEST);
}

void MainScene::Update(float deltaTime) {

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
    glClearColor(0.1f, 0.1f, 0.15f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    m_camera->SetView();

    for(auto gameObject : m_objects) {
        gameObject->Render(m_camera);
    }
    
}

void MainScene::Shutdown() {
    // 리소스 해제
    for(auto gameObject : m_objects) {
        gameObject->Shutdown();
    }
    Physics::Shutdown();
}