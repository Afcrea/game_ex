// SceneManager.h
#pragma once
#include "Scene.h"
#include "lobby_scene.h"
#include "common.h"

class SceneManager {
public:
    static void SetScene(std::unique_ptr<Scene> newScene);
    static void Update(float deltaTime);
    static void Render();
    static void Shutdown();

private:
    static std::unique_ptr<Scene> currentScene;
};
