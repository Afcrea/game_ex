#pragma once
#include "common.h"
#include "scene.h"

CLASS_PTR(GameObject)
class GameObject {
public:
    virtual ~GameObject() = default;

    virtual void Init() = 0;
    virtual void Update(double deltaTime) = 0;
    virtual void Render() = 0;
    virtual void Shutdown() = 0;
};