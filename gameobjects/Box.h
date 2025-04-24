#pragma once
#include "common.h"
#include "gameobject.h"
#include "texture.h"
#include "shader.h"          // Shader 클래스 정의
#include "buffer.h"          // Buffer 클래스 정의
#include "vertex_layout.h"   // VertexLayout 클래스 정의
#include "image.h"           // Image 클래스 정의

CLASS_PTR(Box)
class Box : public GameObject {
public:
    ~Box();
    BoxUPtr static Create();
    void Init() override;
    void Update(double dt) override;
    void Render() override;
    void Shutdown() override;

    void Configure();
    void SetProgramID(uint32_t ID) { programID = ID; }

    ShaderPtr GetVertexShader() { return std::move(vs); }
    ShaderPtr GetFragmentShader() { return std::move(fs); }
    std::vector<float> GetVertices() { return vertices; }
    std::vector<uint32_t> GetIndices() { return indices; }

private:
    Box() {};
    uint32_t programID;
    ShaderUPtr vs;
    ShaderUPtr fs;
    BufferUPtr vbo;
    BufferUPtr ibo;
    VertexLayoutUPtr vao;
    TextureUPtr texture;

    std::vector<float> vertices;
    std::vector<uint32_t> indices;
};