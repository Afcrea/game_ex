#include "skeleton.h"
#include <assimp/scene.h>
#include <glm/glm.hpp>
#include <functional>

// Convert Assimp matrix to glm::mat4
static glm::mat4 AiMatrixToGlm(const aiMatrix4x4& m) {
    return glm::mat4(
        m.a1, m.b1, m.c1, m.d1,
        m.a2, m.b2, m.c2, m.d2,
        m.a3, m.b3, m.c3, m.d3,
        m.a4, m.b4, m.c4, m.d4
    );
}

SkeletonUPtr Skeleton::LoadFromAssimp(const aiScene* scene) {
    if (!scene || !scene->mRootNode) return nullptr;

    SPDLOG_INFO("Skeleton::LoadFromAssimp: Loading skeleton from Assimp scene");

    auto skeleton = std::make_unique<Skeleton>();
    // Root bone name
    skeleton->m_rootBoneName = scene->mRootNode->mName.C_Str();

    // Parse bone info (index + offset matrix) from all meshes
    int boneCounter = 0;
    for (uint32_t mi = 0; mi < scene->mNumMeshes; ++mi) {
        const aiMesh* mesh = scene->mMeshes[mi];
        for (uint32_t bi = 0; bi < mesh->mNumBones; ++bi) {
            const aiBone* bone = mesh->mBones[bi];
            std::string name = bone->mName.C_Str();
            // Avoid duplicates
            if (skeleton->m_boneInfoMap.find(name) == skeleton->m_boneInfoMap.end()) {
                BoneInfo info;
                info.index = boneCounter++;
                info.offsetMatrix = AiMatrixToGlm(bone->mOffsetMatrix);
                skeleton->m_boneInfoMap[name] = info;
            }
        }
    }

    // Recursively parse node hierarchy and default transforms
    std::function<void(const aiNode*, const std::string&)> traverse;
    traverse = [&](const aiNode* node, const std::string& parentName) {
        std::string nodeName = node->mName.C_Str();
        // Store default transform for this node
        skeleton->m_nodeDefaultTransforms[nodeName] = AiMatrixToGlm(node->mTransformation);
        // Build hierarchy map
        if (!parentName.empty()) {
            skeleton->m_boneHierarchy[parentName].push_back(nodeName);
        }
        // Recurse children
        for (uint32_t ci = 0; ci < node->mNumChildren; ++ci) {
            traverse(node->mChildren[ci], nodeName);
        }
    };
    traverse(scene->mRootNode, "");

    return skeleton;
}

// 바인드 포즈 매트릭스 배열 계산기
std::vector<glm::mat4> Skeleton::GetBindPoseMatrices() const {
    size_t boneCount = m_boneInfoMap.size();
    std::vector<glm::mat4> matrices(boneCount, glm::mat4(1.0f));
    // 재귀 시작: 루트 본 이름, identity transform
    BuildBindPose(m_rootBoneName, glm::mat4(1.0f), matrices);
    return matrices;
}

void Skeleton::BuildBindPose(const std::string& boneName,
                             const glm::mat4& parentTransform,
                             std::vector<glm::mat4>& outMatrices) const {
    // 이 노드의 기본(바인드) 로컬 트랜스폼
    glm::mat4 local = GetNodeDefaultTransform(boneName);
    glm::mat4 global = parentTransform * local;

    // 이 본이 실제로 영향력이 있는 본(boneInfoMap에 있으면)이라면
    auto it = m_boneInfoMap.find(boneName);
    if (it != m_boneInfoMap.end()) {
        int idx = it->second.index;
        // 글로벌 × 오프셋 = 스킨 매트릭스
        outMatrices[idx] = global * it->second.offsetMatrix;
    }

    // 자식 본들 재귀
    auto childIt = m_boneHierarchy.find(boneName);
    if (childIt != m_boneHierarchy.end()) {
        for (auto& child : childIt->second) {
            BuildBindPose(child, global, outMatrices);
        }
    }
}