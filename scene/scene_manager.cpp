// SceneManager.cpp
#include "scene_manager.h"

std::unique_ptr<Scene> SceneManager::currentScene = nullptr;

void SceneManager::SetScene(std::unique_ptr<Scene> newScene) {
    if (currentScene) currentScene->Shutdown();
    currentScene = std::move(newScene);
    if (currentScene) currentScene->Init();
}

void SceneManager::Update(double deltaTime) {
    if (currentScene) currentScene->Update(deltaTime);
}

void SceneManager::Render() {
    if (currentScene) currentScene->Render();
}

void SceneManager::Shutdown() {
    if (currentScene) currentScene->Shutdown();
    currentScene.reset();
}
