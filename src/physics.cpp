#include "Physics.h"

static PxDefaultAllocator      gAllocator;
static PxDefaultErrorCallback  gErrorCallback;

PxFoundation* Physics::s_foundation = nullptr;
PxPhysics* Physics::s_sdk = nullptr;
PxScene* Physics::s_scene = nullptr;
PxDefaultCpuDispatcher* Physics::s_dispatcher = nullptr;
PxMaterial* Physics::s_material = nullptr;

void Physics::Initialize() {
    s_foundation = PxCreateFoundation(PX_PHYSICS_VERSION, gAllocator, gErrorCallback);
    s_sdk = PxCreatePhysics(PX_PHYSICS_VERSION, *s_foundation, PxTolerancesScale(), true);

    PxSceneDesc sceneDesc(s_sdk->getTolerancesScale());
    sceneDesc.gravity = PxVec3(0, -9.8f, 0);
    s_dispatcher = PxDefaultCpuDispatcherCreate(2);
    sceneDesc.cpuDispatcher = s_dispatcher;
    sceneDesc.filterShader = PxDefaultSimulationFilterShader;

    s_scene = s_sdk->createScene(sceneDesc);
    s_material = s_sdk->createMaterial(0.5f, 0.5f, 0.6f); // 정지/동 마찰, 반발계수
}

void Physics::StepSimulation(float dt) {
    if (s_scene) {
        s_scene->simulate(dt);
        s_scene->fetchResults(true);
    }
}

void Physics::Shutdown() {
    if (s_material) s_material->release();
    if (s_scene) s_scene->release();
    if (s_dispatcher) s_dispatcher->release();
    if (s_sdk) s_sdk->release();
    if (s_foundation) s_foundation->release();
}
