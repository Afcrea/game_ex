#pragma once

#include "common.h"
#include <assimp/scene.h>

struct aiScene;

// 정보 구조에 맞춰 수정하세요.
struct BoneInfo {
    int index;
    glm::mat4 offsetMatrix;  // inverse bind pose matrix
};

CLASS_PTR(Skeleton);
class Skeleton {
public:
    // root 노드 이름
    const std::string& GetRootBoneName() const { return m_rootBoneName; }
    void SetRootBoneName(const std::string& name) { m_rootBoneName = name; }
    
    // boneName -> BoneInfo
    const std::unordered_map<std::string, BoneInfo>& GetBoneInfoMap() const { return m_boneInfoMap; }

    // boneName -> 자식 본 이름 목록
    const std::unordered_map<std::string, std::vector<std::string>>& GetBoneHierarchy() const { return m_boneHierarchy; }

    // 본이 바인딩 시 기본 transform (aiNode transformation)
    glm::mat4 GetNodeDefaultTransform(const std::string& boneName) const {
        auto it = m_nodeDefaultTransforms.find(boneName);
        return (it != m_nodeDefaultTransforms.end() ? it->second : glm::mat4(1.0f));
    }
    // Assimp scene으로부터 skeleton 정보 생성
    static SkeletonUPtr LoadFromAssimp(const aiScene* scene);


    /// 바인드 포즈 상태의 본 매트릭스 배열을 반환
    std::vector<glm::mat4> GetBindPoseMatrices() const;

private:
// 재귀 헬퍼
    void BuildBindPose(const std::string& boneName,
                       const glm::mat4& parentTransform,
                       std::vector<glm::mat4>& outMatrices) const;
    std::string m_rootBoneName;
    std::unordered_map<std::string, BoneInfo> m_boneInfoMap;
    std::unordered_map<std::string, std::vector<std::string>> m_boneHierarchy;
    std::unordered_map<std::string, glm::mat4> m_nodeDefaultTransforms;

    const aiNode* FindNode(const aiNode* node, const std::string& name) const;
};
