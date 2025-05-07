#pragma once
#include "main_scene.h"
#include "player.h"
#include "backpack.h"
#include "component/renderer_component.h"
#include "component/transform_component.h"
#include "input.h"
#include <PxPhysicsAPI.h>

using namespace physx;

// PhysX 전역 객체
static PxDefaultAllocator gAllocator;
static PxDefaultErrorCallback gErrorCallback;
PxFoundation* gFoundation = nullptr;
PxPhysics* gPhysics = nullptr;
PxScene* gScene = nullptr;
PxMaterial* gMaterial = nullptr;
PxRigidDynamic* gCubeActor = nullptr;
PxDefaultCpuDispatcher* gDispatcher = nullptr;

void MainScene::Init() {
    // 초기화
    auto player = Player::Create();
    gameObjects.push_back(GameObjectPtr(std::move(player)));

    auto backpack = Backpack::Create();
    gameObjects.push_back(GameObjectPtr(std::move(backpack)));

    m_camera = Camera::Create();
    m_camera->Configure(45.0f, (float)m_width / (float)m_height, 0.1f, 100.0f, glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(0.0f, 1.0f, 0.0f));
}

void MainScene::Update(float deltaTime) {
    // 게임 로직 업데이트
    for(auto gameObject : gameObjects) {
        gameObject->Update(deltaTime);
    }
}

void MainScene::Render() {
    glClearColor(0.1f, 0.1f, 0.15f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // OpenGL 상태 초기화
    glDisable(GL_BLEND);
    glDisable(GL_CULL_FACE);
    glDisable(GL_DEPTH_TEST);

    glEnable(GL_BLEND);
    glEnable(GL_CULL_FACE);
    glEnable(GL_DEPTH_TEST);
    
    for(auto gameObject : gameObjects) {
        gameObject->Render(m_camera);
    }
}

void MainScene::Shutdown() {
    // 리소스 해제
    for(auto gameObject : gameObjects) {
        gameObject->Shutdown();
    }
}