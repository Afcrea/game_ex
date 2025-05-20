#include "physx_component.h"
#include "transform_component.h"
#include "linerenderer_component.h"
#include "gameobject.h"

static glm::mat4 PxTransformToGlmMat4(const physx::PxTransform& t, const glm::vec3& scale) {
    // 1) 위치
    glm::vec3 pos{ t.p.x, t.p.y, t.p.z };
    // 2) 회전(quaternion)
    glm::quat rot{ t.q.w, t.q.x, t.q.y, t.q.z };
    // 3) 합성: Translate × Rotate × Scale
    glm::mat4 T = glm::translate(glm::mat4(1.0f), pos);
    glm::mat4 R = glm::toMat4(rot);
    glm::mat4 S = glm::scale(glm::mat4(1.0f), scale);
    return T * R * S;
}

PhysXComponent::~PhysXComponent() {
    Shutdown();
}

PhysXComponentPtr PhysXComponent::Create() {
    auto physx = PhysXComponentPtr(new PhysXComponent());
    return std::move(physx);
}

void PhysXComponent::Configure(bool isDynamic, float mass, float offset) {
    m_offset = offset; 
    m_isDynamic = isDynamic; 
    m_mass = mass; 
}

void PhysXComponent::Configure(float staticFriction, float dynamicFriction, float restitution) { 
    m_staticFriction = staticFriction; 
    m_dynamicFriction = dynamicFriction; 
    m_restitution = restitution;
}

PxRigidDynamic* PhysXComponent::GetDynamicActor() const {
    if (!m_isDynamic || !m_actor) return nullptr;
    return static_cast<PxRigidDynamic*>(m_actor);
}

void PhysXComponent::Init() {
    auto transform = m_owner->GetComponent<TransformComponent>();
    if (!transform) return;

    glm::vec3 position = transform->GetPosition();
    glm::vec3 scale = transform->GetScale();

    glm::mat4 model = transform->GetTransformMatrix();

    glm::vec4 localPos = glm::vec4(position, 1.0f);
    glm::vec4 worldPos = model * localPos;

    glm::vec4 localScale = glm::vec4(scale, m_offset);
    glm::vec4 worldScale = model * localScale;

    PxBoxGeometry geometry(worldScale.x * 0.5f, worldScale.y * 0.5f, worldScale.z * 0.5f);
    
    PxTransform physxTransform(PxVec3(worldPos.x, worldPos.y, worldPos.z));
    PxMaterial* material = Physics::GetSDK()->createMaterial(m_staticFriction, m_dynamicFriction, m_restitution);

    if (m_isTrigger) {
        PxRigidStatic* actor = Physics::GetSDK()->createRigidStatic(physxTransform);
        PxRigidActorExt::createExclusiveShape(*actor, geometry, *material, PxShapeFlag::eTRIGGER_SHAPE);
        m_actor = actor;
    } else if (m_isDynamic) {
        m_actor = PxCreateDynamic(*Physics::GetSDK(), physxTransform, geometry, *material, m_mass);
        if (auto dyn = m_actor->is<physx::PxRigidDynamic>()) {
            dyn->setActorFlag(physx::PxActorFlag::eDISABLE_GRAVITY, false);

            dyn->setRigidDynamicLockFlag(
                physx::PxRigidDynamicLockFlag::eLOCK_ANGULAR_X, true);
            dyn->setRigidDynamicLockFlag(
                physx::PxRigidDynamicLockFlag::eLOCK_ANGULAR_Y, true);
            dyn->setRigidDynamicLockFlag(
                physx::PxRigidDynamicLockFlag::eLOCK_ANGULAR_Z, true);
        }
    } else {
        m_actor = PxCreateStatic(*Physics::GetSDK(), physxTransform, geometry, *material);
    }

    m_actor->userData = m_owner;

    Physics::GetScene()->addActor(*m_actor);
}

void PhysXComponent::Update(float dt) {
    if (!m_actor) return;

    //———— 1) Velocity 보정 (Dynamic에만)
    if (auto dyn = m_actor->is<PxRigidDynamic>()) {
        PxVec3 v = dyn->getLinearVelocity();
        if (v.y > 0.0f) {
            v.y = 0.0f;
            dyn->setLinearVelocity(v);
        }
    }

    // ———— 2) TransformComponent에 PhysX pose 반영
    PxTransform pose = m_actor->getGlobalPose();
    auto transform = m_owner->GetComponent<TransformComponent>();
    if (transform) {
        // 기존 로컬 스케일 유지
        glm::vec3 scl = transform->GetScale();
        glm::mat4 modelMat = PxTransformToGlmMat4(pose, scl);

        // 월드 매트릭스를 통째로 설정
        transform->SetWorldTransformMatrix(modelMat);
        // SPDLOG_INFO("PhysXComponent::Update() - actor: {}, position = ({:.2f},{:.2f},{:.2f})",
        //     m_actor->getConcreteTypeName(), pose.p.x, pose.p.y, pose.p.z);
    }

    // ———— 3) Box corner 계산 & LineRenderer에 전달
    // PxShape가 여러 개일 수 있으니 배열로 받고 반복
    PxShape* shapes[8];
    int shapeCount = m_actor->getShapes(shapes, 8);

    std::vector<glm::vec3> lines;
    for (int i = 0; i < shapeCount; ++i) {
        PxShape* shape = shapes[i];
        PxGeometryHolder gh = shape->getGeometry();
        if (gh.getType() != PxGeometryType::eBOX) 
            continue; // Box 아닌 경우 스킵

        // BoxGeometry, half‐extents
        PxBoxGeometry  boxGeom = gh.box();
        PxVec3 half = boxGeom.halfExtents;

        // 월드 매트릭스 (ActorPose × ShapeLocalPose)
        PxTransform actorPose = m_actor->getGlobalPose();
        PxTransform localPose = shape->getLocalPose();
        PxTransform worldPose = actorPose * localPose;

        // 로컬 코너 8개
        PxVec3 localCorners[8] = {
            {-half.x, -half.y, -half.z}, { half.x, -half.y, -half.z},
            {-half.x,  half.y, -half.z}, { half.x,  half.y, -half.z},
            {-half.x, -half.y,  half.z}, { half.x, -half.y,  half.z},
            {-half.x,  half.y,  half.z}, { half.x,  half.y,  half.z},
        };

        // 월드 코너로 변환
        glm::vec3 worldCorners[8];
        for (int j = 0; j < 8; ++j) {
            PxVec3 p = worldPose.transform(localCorners[j]);
            worldCorners[j] = { p.x, p.y, p.z };
        }

        // 엣지 인덱스(12개 선분)
        static const int edges[12][2] = {
            {0,1},{1,3},{3,2},{2,0},
            {4,5},{5,7},{7,6},{6,4},
            {0,4},{1,5},{2,6},{3,7},
        };
        for (auto& e : edges) {
            lines.push_back(worldCorners[e[0]]);
            lines.push_back(worldCorners[e[1]]);
        }
    }

    // LineRendererComponent에 전달
    if (auto lineComp = m_owner->GetComponent<LineRendererComponent>()) {
        lineComp->SetLines(lines);
    }
}

void PhysXComponent::Shutdown() {
    if (m_actor) {
        SPDLOG_INFO("PhysXComponent::Shutdown() - Destroying actor: {}", m_actor->getConcreteTypeName());
        m_actor->release();
        m_actor = nullptr;
    }
}