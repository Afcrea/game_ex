#pragma once

#include "common.h"
#include "mesh.h"
#include "skeleton.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

CLASS_PTR(Model);
class Model {
public:
    static ModelUPtr Load(const std::string& filename);

    int GetMeshCount() const { return (int)m_meshes.size(); }
    MeshPtr GetMesh(int index) const { return m_meshes[index]; }
    void Draw(const Program* program) const;

    SkeletonPtr GetSkeleton() const { return m_skeleton; }

private:
    Model() {}
    bool LoadByAssimp(const std::string& filename);
    void ProcessMesh(aiMesh* mesh, const aiScene* scene);
    void ProcessNode(aiNode* node, const aiScene* scene);
        
    std::vector<MaterialPtr> m_materials;
    std::vector<MeshPtr> m_meshes;

    SkeletonPtr m_skeleton;
};

CLASS_PTR(Animation);
class Animation {
public:

    struct Keyframe {
        float time;
        glm::vec3 position;
        glm::quat rotation;
        glm::vec3 scale;
    };

    struct BoneAnimation {
        std::string boneName;
        std::vector<Keyframe> keyframes; // 시간순
    };

    float GetDuration() const { return duration; }
    float GetTicksPerSecond() const { return ticksPerSecond; }
    std::unordered_map<std::string, BoneAnimation>& GetBoneAnimations() { return boneAnimations; }
    const std::unordered_map<std::string, BoneAnimation>& GetBoneAnimation() const { return boneAnimations;}

    static AnimationUPtr Load(const std::string& filename);
private:
    Animation() {}
    bool LoadByAssimp(const std::string& filename);

    std::string name;
    float duration;
    float ticksPerSecond;
    std::unordered_map<std::string, BoneAnimation> boneAnimations;
};