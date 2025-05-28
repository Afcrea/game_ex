#include "triggerwall_component.h"
#include "player_component.h"
#include "transform_component.h"
#include "physx_component.h"
#include "animator_component.h"
#include "input.h"
#include "backpack/backpack.h"

TriggerWallComponent::~TriggerWallComponent() {
}

TriggerWallComponentPtr TriggerWallComponent::Create() {
    auto triggerwall = TriggerWallComponentPtr(new TriggerWallComponent());
    return std::move(triggerwall);
}

void TriggerWallComponent::Init() {
    ShaderPtr fs = Shader::CreateFromFile("Resource/lighting.fs", GL_FRAGMENT_SHADER);
    ShaderPtr vs = Shader::CreateFromFile("Resource/lighting.vs", GL_VERTEX_SHADER);
    modelBackpack = Model::Load("Resource/backpackModel/backpack.obj");
    programBackpack = Program::CreateShared({fs, vs});
}

void TriggerWallComponent::Update(float dt) {
    if(m_hasPending < 2) return;
        m_hasPending = 0;
        for(auto changeObject : changeObjects) {
            auto physx = changeObject->GetComponent<PhysXComponent>();
            if (!physx) return;

            physx::PxRigidDynamic* actor = static_cast<physx::PxRigidDynamic*>(physx->GetActor());
            if (!actor) return;
            float moveSpeed = 5.0f;
            if (auto st = actor->is<PxRigidStatic>()) {
                auto transform = changeObject->GetComponent<TransformComponent>();
                if (!transform) return;

                glm::mat4 model = transform->GetTransformMatrix();
                glm::vec3 position = model[3];

                glm::vec3 newPos = glm::vec3(position.x, position.y, position.z + (diff * count));

                PxTransform newPose(PxVec3(newPos.x, newPos.y, newPos.z),
                                    st->getGlobalPose().q);
                // Static은 simulate() 뒤에도 유지되므로, 언제든 setGlobalPose OK
                st->setGlobalPose(newPose);
                transform->SetPosition(newPos);
                SPDLOG_INFO("TriggerWallComponent::Update - Ground position updated to: ({}, {}, {})", newPos.x, newPos.y, newPos.z);
            }
        }

        auto myPhysx = m_owner->GetComponent<PhysXComponent>();
        if (!myPhysx) return;

        physx::PxRigidDynamic* myActor = static_cast<physx::PxRigidDynamic*>(myPhysx->GetActor());
        if (!myActor) return;

        if (auto stt = myActor->is<PxRigidStatic>()) {
            auto transform = m_owner->GetComponent<TransformComponent>();
            if (!transform) return;

            PxTransform myPose = myActor->getGlobalPose();
            glm::vec3 myPosition = glm::vec3(myPose.p.x, myPose.p.y, myPose.p.z + diff);

            PxTransform newPose(PxVec3(myPosition.x, myPosition.y, myPosition.z),
                                    stt->getGlobalPose().q);

            stt->setGlobalPose(newPose);
            transform->SetPosition(myPosition);
        }
        


}

void TriggerWallComponent::OnTriggerEnter(GameObject* other) {
    if (other->GetName() == "Player") {
        auto objects = m_owner->GetScene()->GetAllObjects();
        count = 0;
        minZ = maxZ;
        m_hasPending++;
        if(m_hasPending < 2) return;
        changeObjects.clear();
        GameObjectPtr ground = nullptr;
        for (const auto& obj : objects) {
            if (obj->GetName().find("Ground") != std::string::npos) {
                auto transform = obj->GetComponent<TransformComponent>();
                if (transform) {
                    //SPDLOG_INFO("TriggerWallComponent::OnTriggerEnter - Found Ground object: {}", obj->GetName());
                    count++;
                    if(maxZ < transform->GetPosition().z) {
                        maxZ = transform->GetPosition().z;
                    }
                    // 그라운드간의 z 위치 차이
                    //SPDLOG_INFO("minz: {}, transform z: {}", minZ, transform->GetPosition().z);
                    if(minZ >= transform->GetPosition().z) {
                        minZ = transform->GetPosition().z;
                        ground = obj;
                    }
                }
            }
        }
        changeObjects.push_back(ground);
        for (const auto& obj : objects) {
            if(obj->GetName().find("Wall") != std::string::npos) {
                if(obj->GetName() == "TriggerWall") continue; // TriggerWall은 제외
                auto transform = obj->GetComponent<TransformComponent>();
                if (transform) {
                    if(minZ == transform->GetPosition().z) {
                        changeObjects.push_back(obj);
                    }
                }
            }
        }
        auto tarZ = m_owner->GetComponent<TransformComponent>()->GetPosition().z;
        auto currScene = m_owner->GetScene();
        currScene->RequestSpawn<Backpack>(glm::vec3(5.0f, 2.0f, (tarZ + diff) /2), modelBackpack, programBackpack);
        currScene->RequestSpawn<Backpack>(glm::vec3(-5.0f, 2.0f, (tarZ + diff)/2), modelBackpack, programBackpack);
    }
    
}