#include "skeleton.h"
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

    skeleton->m_boneHierarchy.clear();

    // 2) boneInfoMap에 있는 모든 본에 대해, aiNode 트리에서 해당 노드를 찾고
    //    부모 노드로 거슬러 올라가면서 첫 번째로 boneInfoMap에 있는 노드를 parent로 간주
    for (auto& [boneName, info] : skeleton->m_boneInfoMap) {
        const aiNode* node = skeleton->FindNode(scene->mRootNode, boneName);
        if (!node) continue;

        const aiNode* parent = node->mParent;
        // boneInfoMap에 등록된 부모가 나올 때까지 계속 위로
        while (parent && skeleton->m_boneInfoMap.count(parent->mName.C_Str()) == 0) {
            parent = parent->mParent;
        }

        if (parent && skeleton->m_boneInfoMap.count(parent->mName.C_Str())) {
            // parent->mName 은 std::string이므로 .C_Str() 대신 그대로 사용
            skeleton->m_boneHierarchy[parent->mName.C_Str()]
                                .push_back(boneName);
        }
        // parent가 없으면(bone이 최상위라면) 자식 없이 넘어갑니다
    }

    // 3) m_rootBoneName 결정: boneInfoMap 중 parent를 못 찾은(최상위) 본
    for (auto& [boneName, info] : skeleton->m_boneInfoMap) {
        // 이 본이 어떤 parent 의 child로도 등록되지 않았다면
        bool isChild = false;
        for (auto& [p, kids] : skeleton->m_boneHierarchy) {
            if (std::find(kids.begin(), kids.end(), boneName) != kids.end()) {
                isChild = true;
                break;
            }
        }
        if (!isChild) {
            skeleton->m_rootBoneName = boneName;
            SPDLOG_INFO("Detected real root bone: {}", boneName);
            break;
        }
    }

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

const aiNode* Skeleton::FindNode(const aiNode* node, const std::string& name) const {
    if (!node) return nullptr;
    if (node->mName.C_Str() == name) return node;
    for (uint32_t i = 0; i < node->mNumChildren; ++i) {
        if (auto f = FindNode(node->mChildren[i], name); f)
            return f;
    }
    return nullptr;
}