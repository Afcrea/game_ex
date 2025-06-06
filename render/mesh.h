#pragma once

#include "common.h"
#include "buffer.h"
#include "vertex_layout.h"
#include "program.h"
#include "texture.h"

struct Vertex {
    glm::vec3 position;
    glm::vec3 normal;
    glm::vec2 texCoord;
    glm::ivec4 boneIDs;
    glm::vec4 weights;
};

CLASS_PTR(Material);
class Material {
public:
    static MaterialUPtr Create() {
        return MaterialUPtr(new Material());
    }
    TexturePtr diffuse;
    TexturePtr specular;
    float shininess { 32.0f };

    void SetToProgram(const Program* program) const;

private:
    Material() {}
};

CLASS_PTR(Mesh);
class Mesh {
public:
    static MeshUPtr Create(
        const std::vector<Vertex>& vertices,
        const std::vector<uint32_t>& indices,
        uint32_t primitiveType);
    static MeshUPtr CreateBox();

    void SetMaterial(MaterialPtr material) { m_material = material; }
    MaterialPtr GetMaterial() const { return m_material; }

    const VertexLayout* GetVertexLayout() const {
        return m_vertexLayout.get();
    }
    BufferPtr GetVertexBuffer() const { return m_vertexBuffer; }
    BufferPtr GetIndexBuffer() const { return m_indexBuffer; }

    void Draw(const Program* program) const;

private:
    Mesh() {}
    void Init(
        const std::vector<Vertex>& vertices,
        const std::vector<uint32_t>& indices,
        uint32_t primitiveType);

    MaterialPtr m_material;

    uint32_t m_primitiveType { GL_TRIANGLES };
    VertexLayoutUPtr m_vertexLayout;
    BufferPtr m_vertexBuffer;
    BufferPtr m_indexBuffer;
};