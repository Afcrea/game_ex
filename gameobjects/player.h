#pragma once
#include "common.h"
#include "gameobject.h"
#include "shader.h"
#include "buffer.h"
#include "vertex_layout.h"

CLASS_PTR(Player)
class Player : public GameObject{
public:
    ~Player();
    PlayerUPtr static Create();
    void Init() override;
    void Update(double dt) override;
    void Render() override;
    void Shutdown() override;

    void Configure();
    
    ShaderPtr GetVertexShader() { return std::move(vs); }
    ShaderPtr GetFragmentShader() { return std::move(fs); }
    std::vector<float> GetVertices() { return vertices; }
    std::vector<uint32_t> GetIndices() { return indices; }

    void SetProgramID(uint32_t ID) { programID = ID; }
private:
    Player() {};
    uint32_t m_vertexArrayObject { 0 };
    uint32_t m_vertexBuffer { 0 };
    uint32_t m_indexBuffer { 0 };
    uint32_t m_program { 0 };
    
    ShaderUPtr fs;
    ShaderUPtr vs;

    uint32_t programID;
    BufferUPtr playerVBO;
    BufferUPtr playerIBO;
    VertexLayoutUPtr playerVAO;

    std::vector<uint32_t>indices;
    std::vector<float>vertices;
};