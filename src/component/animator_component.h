#pragma once

#include "common.h"
#include "component.h"
#include "model.h"
#include "skeleton.h"

CLASS_PTR(AnimatorComponent);
class AnimatorComponent : public Component {
public:
    static AnimatorComponentUPtr Create();
    ~AnimatorComponent();

    void Configure(const std::string& filename);
    void SetCurrentAnimation(int index) { m_currentAnimationIndex = index; }
    const std::vector<glm::mat4>& GetBoneMatrices() { return m_boneMatrices; }
    const std::string& GetSkeletonRootName() { return m_skeletonRootName; }
    const AnimationPtr& GetCurrentAnimation() { return m_animations[m_currentAnimationIndex]; }
    
    void Update(float deltaTime) override;
private:
    std::vector<AnimationPtr> m_animations;
    int m_currentAnimationIndex = 0;
    float m_currentTime = 0.0f;
    std::vector<glm::mat4> m_boneMatrices;
    SkeletonPtr m_skeleton;

    glm::mat4 m_globalInverseTransform;

    std::string m_skeletonRootName;

    glm::vec3 InterpolatePosition(const std::vector<Animation::KeyPosition>& keys, float time);
    glm::quat InterpolateRotation(const std::vector<Animation::KeyRotation>& keys, float time);
    glm::vec3 InterpolateScale(const std::vector<Animation::KeyScale>& keys, float time);
    void ProcessBoneRecursive(const std::string& boneName, const glm::mat4& parentTransform, float timeInTicks);
};