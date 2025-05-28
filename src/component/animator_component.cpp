#include "animator_component.h"
#include "renderer_component.h"
#include "transform_component.h"
#include "gameobject.h"

AnimatorComponentPtr AnimatorComponent::Create() {
    auto animator = AnimatorComponentPtr(new AnimatorComponent());
    return std::move(animator);
}

AnimatorComponent::~AnimatorComponent() {
}

void AnimatorComponent::Configure(const std::string& filename) {
    m_skeleton = m_owner->GetComponent<RendererComponent>()->GetModel()->GetSkeleton();

    if (!m_skeleton) return;
    auto anim = Animation::Load(filename);
    if (!anim) return;

    // 스켈레톤에 없는 본 이름은 애니메이션에서 제외
    const auto& boneInfo = m_skeleton->GetBoneInfoMap();
    auto boneAnim = anim->GetBoneAnimation();

    // 남은 채널이 있으면 등록
    if (!boneAnim.empty()) {
        m_animations.push_back(std::move(anim));
        m_boneMatrices.resize(boneInfo.size());
    }
}

// Interpolation helpers
glm::vec3 AnimatorComponent::InterpolatePosition(const std::vector<Animation::KeyPosition>& keys,float time) {
    if (keys.empty()) return glm::vec3(0.0f);
    if (keys.size() == 1) return keys[0].value;

    // 현재 time 에서 앞뒤 키 인덱스 찾기
    uint32_t i = 0;
    while (i + 1 < keys.size() && time > keys[i+1].time) ++i;

    float t0 = keys[i].time;
    float t1 = keys[i+1].time;
    float factor = (time - t0) / (t1 - t0);
    return glm::mix(keys[i].value, keys[i+1].value, factor);
}

glm::quat AnimatorComponent::InterpolateRotation(const std::vector<Animation::KeyRotation>& keys, float time) {
    if (keys.empty()) return glm::quat(1,0,0,0);
    if (keys.size() == 1) return keys[0].value;

    uint32_t i = 0;
    while (i + 1 < keys.size() && time > keys[i+1].time) ++i;

    float t0 = keys[i].time;
    float t1 = keys[i+1].time;
    float factor = (time - t0) / (t1 - t0);
    return glm::slerp(keys[i].value, keys[i+1].value, factor);
}

glm::vec3 AnimatorComponent::InterpolateScale(const std::vector<Animation::KeyScale>& keys, float time) {
    if (keys.empty()) return glm::vec3(1.0f);
    if (keys.size() == 1) return keys[0].value;

    uint32_t i = 0;
    while (i + 1 < keys.size() && time > keys[i+1].time) ++i;

    float t0 = keys[i].time;
    float t1 = keys[i+1].time;
    float factor = (time - t0) / (t1 - t0);
    return glm::mix(keys[i].value, keys[i+1].value, factor);
}

// Recursive processing of bones
void AnimatorComponent::ProcessBoneRecursive(const std::string& boneName, const glm::mat4& parentTransform, float timeInTicks) {
    // 1) 본의 로컬 트랜스폼 결정
    //    - 애니메이션 채널이 있으면 animLocal  
    //    - 없으면 바인드 포즈 local
    glm::mat4 local;
    auto& baMap = m_animations[m_currentAnimationIndex]->GetBoneAnimations();
    auto it     = baMap.find(boneName);
    if (it != baMap.end()) {
        auto rootBoneName = m_skeleton->GetRootBoneName();

        const auto& ba = it->second;
        glm::vec3 pos = InterpolatePosition(ba.positions, timeInTicks);
        glm::quat rot = InterpolateRotation(ba.rotations, timeInTicks);
        glm::vec3 scl = InterpolateScale(ba.scales, timeInTicks);

        if(rootBoneName == boneName) {
            // 루트 본은 월드 좌표계에서 변환
            pos = m_globalInverseTransform * glm::vec4(0.0f,0.0f,0.0f, 1.0f);
        }

        local = glm::translate(glm::mat4(1.0f), pos)
              * glm::mat4_cast(rot)
              * glm::scale(glm::mat4(1.0f), scl);
    }
    else {
        // 애니메이션 키가 없는 본은 바인드 포즈의 로컬 트랜스폼
        local = m_skeleton->GetNodeDefaultTransform(boneName);
    }

    // 2) 글로벌 트랜스폼
    glm::mat4 global = parentTransform * local;

    // 3) 스킨 매트릭스 업데이트 (애니메이션이 있는 본만 덮어씀)
    auto const& boneInfoMap = m_skeleton->GetBoneInfoMap();
    if (auto infoIt = boneInfoMap.find(boneName); infoIt != boneInfoMap.end()) {
        int idx = infoIt->second.index;
        // offsetMatrix = inverse bind-pose
        m_boneMatrices[idx] = global * infoIt->second.offsetMatrix;
    }

    // 4) 자식 순회 (범위 검사)
    auto const& hier = m_skeleton->GetBoneHierarchy();
    if (auto chIt = hier.find(boneName); chIt != hier.end()) {
        for (auto const& child : chIt->second) {
            ProcessBoneRecursive(child, global, timeInTicks);
        }
    }
}

void AnimatorComponent::Update(float deltaTime) {
    if (m_animations.empty()) return;

    std::fill(m_boneMatrices.begin(), m_boneMatrices.end(), glm::mat4(1.0f));

    m_boneMatrices = m_skeleton->GetBindPoseMatrices();

    // 2) 애니메이션 시간 누적
    auto& anim = m_animations[m_currentAnimationIndex];
    float tps = anim->GetTicksPerSecond() > 0 
                ? anim->GetTicksPerSecond() 
                : 25.0f;
    m_currentTime += deltaTime * tps;
    float timeInTicks = fmod(m_currentTime, anim->GetDuration());

    // 3) 루트 바인드와 글로벌 역변환 준비
    const std::string& rootName = m_skeleton->GetRootBoneName();
    glm::mat4 rootDefault = m_skeleton->GetNodeDefaultTransform(rootName);

    // globalInverseTransform은 초기화 시 미리 계산해 두었다고 가정
    m_globalInverseTransform = inverse(rootDefault);

    // 루트의 월드 변환: 위치/회전/스케일 적용
    glm::mat4 rootBind = rootDefault;
    auto transformCmp = m_owner->GetComponent<TransformComponent>();
    rootBind = glm::translate(rootBind, transformCmp->GetPosition());
    rootBind = glm::translate(rootBind, glm::vec3(0, 0.0f, 0));
    rootBind = glm::rotate   (rootBind, transformCmp->GetRotation().x, {1,0,0});
    rootBind = glm::rotate   (rootBind, transformCmp->GetRotation().y, {0,1,0});
    rootBind = glm::rotate   (rootBind, transformCmp->GetRotation().z, {0,0,1});
    //rootBind = glm::scale    (rootBind, transformCmp->GetScale());

    // 4) 재귀로 본 변환 계산
    ProcessBoneRecursive(rootName, rootBind, timeInTicks);
}