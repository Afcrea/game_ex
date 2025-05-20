#pragma once
#include <PxPhysicsAPI.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/quaternion.hpp>
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

class ContactListener : public physx::PxSimulationEventCallback {
public:
    void onTrigger(physx::PxTriggerPair* pairs, physx::PxU32 count) override;
    void onContact(const physx::PxContactPairHeader&, const physx::PxContactPair*, physx::PxU32) override {}
    void onConstraintBreak(physx::PxConstraintInfo*, physx::PxU32) override {}
    void onWake(physx::PxActor**, physx::PxU32) override {}
    void onSleep(physx::PxActor**, physx::PxU32) override {}
    void onAdvance(const physx::PxRigidBody* const*, const physx::PxTransform*, const physx::PxU32) override {}
};