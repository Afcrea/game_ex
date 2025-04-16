// Scene.h
#pragma once


class Scene {
public:
    virtual ~Scene() = default;

    virtual void Init() = 0;
    virtual void Update(double deltaTime) = 0;
    virtual void Render() = 0;
    virtual void Shutdown() = 0;
};
