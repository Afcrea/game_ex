#pragma once
#include <PxPhysicsAPI.h>

using namespace physx;

class Physics {
public:
    static void Initialize();
    static void Shutdown();
    static void StepSimulation(float deltaTime);

    static PxPhysics* GetSDK() { return s_sdk; }
    static PxScene* GetScene() { return s_scene; }
    static PxMaterial* GetDefaultMaterial() { return s_material; }

private:
    static PxFoundation* s_foundation;
    static PxPhysics* s_sdk;
    static PxScene* s_scene;
    static PxDefaultCpuDispatcher* s_dispatcher;
    static PxMaterial* s_material;
};
