#include "model.h"
#include "texture.h"

ModelUPtr Model::Load(const std::string& filename) {
    auto model = ModelUPtr(new Model());
    if (!model->LoadByAssimp(filename))
      return nullptr;
    return std::move(model);
}

AnimationUPtr Animation::Load(const std::string& filename) {
    auto animation = AnimationUPtr(new Animation());
    if (!animation->LoadByAssimp(filename))
      return nullptr;
    return std::move(animation);
}

bool Animation::LoadByAssimp(const std::string& filename) {
    Assimp::Importer importer;
    const aiScene* scene = importer.ReadFile(
        filename,
        aiProcess_Triangulate |
        aiProcess_FlipUVs    |
        aiProcess_CalcTangentSpace
    );
    if (!scene || scene->mNumAnimations == 0) {
        SPDLOG_ERROR("failed to load animation: {}", filename);
        return false;
    }

    const aiAnimation* aiAnim = scene->mAnimations[0];
    name           = aiAnim->mName.C_Str();
    duration       = static_cast<float>(aiAnim->mDuration);
    ticksPerSecond = aiAnim->mTicksPerSecond != 0.0
                     ? static_cast<float>(aiAnim->mTicksPerSecond)
                     : 25.0f;

    // 채널별(Bone) 처리
    for (uint32_t i = 0; i < aiAnim->mNumChannels; ++i) {
        const aiNodeAnim* channel = aiAnim->mChannels[i];
        BoneAnimation ba;
        ba.boneName = channel->mNodeName.C_Str();

        // 1) KeyPosition 벡터
        ba.positions.reserve(channel->mNumPositionKeys);
        for (uint32_t k = 0; k < channel->mNumPositionKeys; ++k) {
            const auto& pk = channel->mPositionKeys[k];
            ba.positions.push_back({
                static_cast<float>(pk.mTime),
                glm::vec3(pk.mValue.x, pk.mValue.y, pk.mValue.z)
            });
        }

        // 2) KeyRotation 벡터
        ba.rotations.reserve(channel->mNumRotationKeys);
        for (uint32_t k = 0; k < channel->mNumRotationKeys; ++k) {
            const auto& rk = channel->mRotationKeys[k];
            ba.rotations.push_back({
                static_cast<float>(rk.mTime),
                glm::quat(rk.mValue.w, rk.mValue.x, rk.mValue.y, rk.mValue.z)
            });
        }

        // 3) KeyScale 벡터
        ba.scales.reserve(channel->mNumScalingKeys);
        for (uint32_t k = 0; k < channel->mNumScalingKeys; ++k) {
            const auto& sk = channel->mScalingKeys[k];
            ba.scales.push_back({
                static_cast<float>(sk.mTime),
                glm::vec3(sk.mValue.x, sk.mValue.y, sk.mValue.z)
            });
        }

        boneAnimations[ba.boneName] = std::move(ba);
    }

    return true;
}

bool Model::LoadByAssimp(const std::string& filename) {
    Assimp::Importer importer;
    auto scene = importer.ReadFile(filename, aiProcess_Triangulate | aiProcess_FlipUVs);
  
    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
      SPDLOG_ERROR("failed to load model: {}", filename);
      return false;
    }
  
    auto dirname = filename.substr(0, filename.find_last_of("/"));
    auto LoadTexture = [&](aiMaterial* material, aiTextureType type) -> TexturePtr {
        if (material->GetTextureCount(type) <= 0)
            return nullptr;
        aiString filepath;
        material->GetTexture(aiTextureType_DIFFUSE, 0, &filepath);
        auto image = Image::Load(fmt::format("{}/{}", dirname, filepath.C_Str()));
        if (!image)
            return nullptr;
        return Texture::CreateFromImage(image.get());
    };

    for (uint32_t i = 0; i < scene->mNumMaterials; i++) {
        auto material = scene->mMaterials[i];
        auto glMaterial = Material::Create();
        glMaterial->diffuse = LoadTexture(material, aiTextureType_DIFFUSE);
        glMaterial->specular = LoadTexture(material, aiTextureType_SPECULAR);

        if (!glMaterial->diffuse) {
            aiColor3D diffColor(1.0f, 1.0f, 1.0f);
            if (AI_SUCCESS == material->Get(AI_MATKEY_COLOR_DIFFUSE, diffColor)) {
                glm::vec4 col{diffColor.r, diffColor.g, diffColor.b, 1.0f};
                
                auto image = Image::CreateSingleColorImage(1, 1, col);
                glMaterial->diffuse = Texture::CreateFromImage(image.get());
            }
        } 

        m_materials.push_back(std::move(glMaterial));
    }

    bool hasBones = false;
    for (unsigned int i = 0; i < scene->mNumMeshes; ++i) {
        const aiMesh* mesh = scene->mMeshes[i];
        if (mesh->mNumBones > 0) {
            hasBones = true;
            break;
        }
    }

    if (hasBones) {
        SPDLOG_INFO("model: {}, #mesh: {}, #material: {}, #anim: {}", filename, scene->mNumMeshes, scene->mNumMaterials, scene->mNumAnimations);
        m_skeleton = Skeleton::LoadFromAssimp(scene);
    } else {
        m_skeleton = nullptr;
    }
    
    
    ProcessNode(scene->mRootNode, scene);

    return true;
}

void Model::ProcessNode(aiNode* node, const aiScene* scene) {
    for (uint32_t i = 0; i < node->mNumMeshes; i++) {
      auto meshIndex = node->mMeshes[i];
      auto mesh = scene->mMeshes[meshIndex];
      ProcessMesh(mesh, scene);
    }
  
    for (uint32_t i = 0; i < node->mNumChildren; i++) {
      ProcessNode(node->mChildren[i], scene);
    }
}

void Model::ProcessMesh(aiMesh* mesh, const aiScene* scene) {
    //SPDLOG_INFO("process mesh: {}, #vert: {}, #face: {}", mesh->mName.C_Str(), mesh->mNumVertices, mesh->mNumFaces);
  
    std::vector<Vertex> vertices;
    vertices.resize(mesh->mNumVertices);
    for (uint32_t i = 0; i < mesh->mNumVertices; i++) {
        auto& v = vertices[i];
        v.position = glm::vec3(mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z);

        if (mesh->mNormals) {
            v.normal = glm::vec3(mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z);
        } else {
            v.normal = glm::vec3(0.0f, 0.0f, 0.0f);
        }

        if (mesh->mTextureCoords[0]) {
            v.texCoord = glm::vec2(mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y);
        } else {
            v.texCoord = glm::vec2(0.0f, 0.0f);
        }

        v.boneIDs = glm::ivec4(0);
        v.weights = glm::vec4(0.0f);
    }

    for (uint32_t boneIndex = 0; boneIndex < mesh->mNumBones; boneIndex++) {
        aiBone* bone = mesh->mBones[boneIndex];
        for (uint32_t w = 0; w < bone->mNumWeights; w++) {
            uint32_t vertexId = bone->mWeights[w].mVertexId;
            float weight = bone->mWeights[w].mWeight;

            for (int i = 0; i < 4; i++) {
                if (vertices[vertexId].weights[i] == 0.0f) {
                    vertices[vertexId].boneIDs[i] = boneIndex;
                    vertices[vertexId].weights[i] = weight;
                    break;
                }
            }
        }
    }
  
    std::vector<uint32_t> indices;
    indices.resize(mesh->mNumFaces * 3);
    for (uint32_t i = 0; i < mesh->mNumFaces; i++) {
        indices[3*i  ] = mesh->mFaces[i].mIndices[0];
        indices[3*i+1] = mesh->mFaces[i].mIndices[1];
        indices[3*i+2] = mesh->mFaces[i].mIndices[2];
    }
  
    auto glMesh = Mesh::Create(vertices, indices, GL_TRIANGLES);
    if (mesh->mMaterialIndex >= 0)
        glMesh->SetMaterial(m_materials[mesh->mMaterialIndex]);

    m_meshes.push_back(std::move(glMesh));
}

void Model::Draw(const Program* program) const {
    for (auto& mesh: m_meshes) {
      mesh->Draw(program);
    }
}