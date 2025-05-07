// SceneManager.h
#pragma once
#include "Scene.h"
#include "lobby_scene.h"
#include "common.h"

class SceneManager {
public:
    static void SetScene(SceneUPtr newScene);
    static void Update(float deltaTime);
    static void Render();
    static void Shutdown();

private:
    static SceneUPtr currentScene;
};
