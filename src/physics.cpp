#include "Physics.h"
#include "gameobject.h"
#include "component/component.h"

static PxDefaultAllocator      gAllocator;
static PxDefaultErrorCallback  gErrorCallback;

PxFoundation* Physics::s_foundation = nullptr;
PxPhysics* Physics::s_sdk = nullptr;
PxScene* Physics::s_scene = nullptr;
PxDefaultCpuDispatcher* Physics::s_dispatcher = nullptr;
PxMaterial* Physics::s_material = nullptr;
static ContactListener g_contactListener;

void Physics::Initialize() {
    s_foundation = PxCreateFoundation(PX_PHYSICS_VERSION, gAllocator, gErrorCallback);
    s_sdk = PxCreatePhysics(PX_PHYSICS_VERSION, *s_foundation, PxTolerancesScale(), true);

    PxSceneDesc sceneDesc(s_sdk->getTolerancesScale());
    sceneDesc.gravity = PxVec3(0, -9.8f, 0);
    s_dispatcher = PxDefaultCpuDispatcherCreate(2);
    sceneDesc.cpuDispatcher = s_dispatcher;
    sceneDesc.filterShader = PxDefaultSimulationFilterShader;
    sceneDesc.simulationEventCallback = &g_contactListener;

    s_scene = s_sdk->createScene(sceneDesc);
    s_material = s_sdk->createMaterial(1.0f, 1.0f, 0.0f); // 정지/동 마찰, 반발계수
}

void Physics::StepSimulation(float dt) {
    if (s_scene) {
        s_scene->simulate(dt);
        s_scene->fetchResults(true);
    }
}

void Physics::Simulation(float dt) {
    if(s_scene) 
        s_scene->simulate(dt);
}

void Physics::FetchResults() {
    if(s_scene)
        s_scene->fetchResults(true);
}

void Physics::Shutdown() {
    if (s_material) s_material->release();
    if (s_scene) s_scene->release();
    if (s_dispatcher) s_dispatcher->release();
    if (s_sdk) s_sdk->release();
    if (s_foundation) s_foundation->release();
}

void ContactListener::onTrigger(PxTriggerPair* pairs, PxU32 count) {
    for (PxU32 i = 0; i < count; ++i) {
        auto triggerActor = pairs[i].triggerActor;
        auto otherActor = pairs[i].otherActor;

        auto triggerObj = static_cast<GameObject*>(triggerActor->userData);
        auto otherObj = static_cast<GameObject*>(otherActor->userData);

        if (!triggerObj || !otherObj) continue;
        if (triggerObj->IsPendingDestroy() || otherObj->IsPendingDestroy()) continue;

        for (auto& comp : otherObj->GetAllComponents()) {
            comp.second->OnTriggerEnter(triggerObj);
        }
        for (auto& comp : triggerObj->GetAllComponents()) {
            comp.second->OnTriggerEnter(otherObj);
        }
    }
}