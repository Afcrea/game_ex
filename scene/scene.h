// Scene.h
#pragma once
#include "common.h"


CLASS_PTR(Scene)
class Scene {
public:
    virtual ~Scene() = default;

    virtual void Init() = 0;
    virtual void Update(float deltaTime) = 0;
    virtual void Render() = 0;
    virtual void Shutdown() = 0;
};
