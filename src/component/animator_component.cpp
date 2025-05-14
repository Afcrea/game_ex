#include "animator_component.h"
#include "renderer_component.h"
#include "transform_component.h"
#include "gameobject.h"

AnimatorComponentUPtr AnimatorComponent::Create() {
    auto animator = AnimatorComponentUPtr(new AnimatorComponent());
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
    for (auto it = boneAnim.begin(); it != boneAnim.end(); ) {
        if (boneInfo.find(it->first) == boneInfo.end()) {
            spdlog::warn("Animation '{}' has channel '{}' which is not in skeleton → skipping", filename, it->first);
            it = boneAnim.erase(it);
        } else {
            ++it;
        }
    }

    // 남은 채널이 있으면 등록
    if (!boneAnim.empty()) {
        m_animations.push_back(std::move(anim));
        m_boneMatrices.resize(boneInfo.size());
    }
}

// Interpolation helpers
glm::vec3 AnimatorComponent::InterpolatePosition(const std::vector<Animation::Keyframe>& keys, float time) {
    if (keys.empty()) return glm::vec3(0.0f);
    if (keys.size() == 1) return keys[0].position;

    // Find pair of keys surrounding time
    size_t index = 0;
    for (size_t i = 0; i + 1 < keys.size(); ++i) {
        if (time < keys[i+1].time) {
            index = i;
            break;
        }
    }
    const auto& keyA = keys[index];
    const auto& keyB = keys[std::min(index + 1, keys.size()-1)];
    float dt = keyB.time - keyA.time;
    float factor = dt > 0.0f ? (time - keyA.time) / dt : 0.0f;
    return glm::mix(keyA.position, keyB.position, factor);
}

glm::quat AnimatorComponent::InterpolateRotation(const std::vector<Animation::Keyframe>& keys, float time) {
    if (keys.empty()) return glm::quat();
    if (keys.size() == 1) return keys[0].rotation;

    size_t index = 0;
    for (size_t i = 0; i + 1 < keys.size(); ++i) {
        if (time < keys[i+1].time) {
            index = i;
            break;
        }
    }
    const auto& keyA = keys[index];
    const auto& keyB = keys[std::min(index + 1, keys.size()-1)];
    float dt = keyB.time - keyA.time;
    float factor = dt > 0.0f ? (time - keyA.time) / dt : 0.0f;
    return glm::slerp(keyA.rotation, keyB.rotation, factor);
}

glm::vec3 AnimatorComponent::InterpolateScale(const std::vector<Animation::Keyframe>& keys, float time) {
    if (keys.empty()) return glm::vec3(1.0f);
    if (keys.size() == 1) return keys[0].scale;

    size_t index = 0;
    for (size_t i = 0; i + 1 < keys.size(); ++i) {
        if (time < keys[i+1].time) {
            index = i;
            break;
        }
    }
    const auto& keyA = keys[index];
    const auto& keyB = keys[std::min(index + 1, keys.size()-1)];
    float dt = keyB.time - keyA.time;
    float factor = dt > 0.0f ? (time - keyA.time) / dt : 0.0f;
    return glm::mix(keyA.scale, keyB.scale, factor);
}

// Recursive processing of bones
void AnimatorComponent::ProcessBoneRecursive(const std::string& boneName, const glm::mat4& parentTransform, float timeInTicks) {
    // 1) 로컬 트랜스폼
    //glm::mat4 local(1.0f);
    glm::mat4 local = m_owner->GetComponent<TransformComponent>()->GetTransformMatrix();
    const auto& anim     = m_animations[m_currentAnimationIndex];
    const auto& boneAnims= anim->GetBoneAnimations();
    auto itBA = boneAnims.find(boneName);
    if (itBA != boneAnims.end() && !itBA->second.keyframes.empty()) {
        const auto& ka = itBA->second.keyframes;
        local = glm::translate(glm::mat4(1.0f), InterpolatePosition(ka, timeInTicks))
            * glm::mat4_cast( InterpolateRotation(ka, timeInTicks))
            * glm::scale(glm::mat4(1.0f), InterpolateScale(ka, timeInTicks));
    } else {
        local = m_skeleton->GetNodeDefaultTransform(boneName);
    }

    // 2) 글로벌 트랜스폼
    glm::mat4 global = parentTransform * local;

    // 3) 스킨 매트릭스
    const auto& boneInfoMap = m_skeleton->GetBoneInfoMap();
    auto itInfo = boneInfoMap.find(boneName);
    if (itInfo != boneInfoMap.end()) {
        int idx = itInfo->second.index;
        if (idx >= 0 && idx < (int)m_boneMatrices.size())
            m_boneMatrices[idx] = global * itInfo->second.offsetMatrix;
    }

    // 4) 자식 순회
    static const std::vector<std::string> empty;
    const auto& hier = m_skeleton->GetBoneHierarchy();
    auto itCh = hier.find(boneName);
    const auto& children = (itCh != hier.end()) ? itCh->second : empty;
    for (auto& cn : children) {
        ProcessBoneRecursive(cn, global, timeInTicks);
    }
}

void AnimatorComponent::Update(float deltaTime) {
    if (m_animations.empty()) return;

    auto& anim = m_animations[m_currentAnimationIndex];

    m_currentTime += deltaTime * anim->GetTicksPerSecond();
    float timeInTicks = fmod(m_currentTime, anim->GetDuration());

    const std::string& root = m_skeleton->GetRootBoneName();
    glm::mat4 rootBind = m_skeleton->GetNodeDefaultTransform(root);
    auto transform = m_owner->GetComponent<TransformComponent>();
    glm::vec3 pos = transform->GetPosition();
    glm::vec3 rot = transform->GetRotation(); // ← radians
    glm::vec3 scale = transform->GetScale();

    rootBind = glm::translate(rootBind, pos);
    rootBind = glm::rotate(rootBind, rot.x, glm::vec3(1, 0, 0));
    rootBind = glm::rotate(rootBind, rot.y, glm::vec3(0, 1, 0));
    rootBind = glm::rotate(rootBind, rot.z, glm::vec3(0, 0, 1));
    //rootBind = glm::scale(rootBind, scale);
    ProcessBoneRecursive(root, rootBind, timeInTicks);
}