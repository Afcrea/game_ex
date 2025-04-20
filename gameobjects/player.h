#pragma once
#include "common.h"
#include "gameobject.h"

CLASS_PTR(Player)
class Player : public GameObject{
public:
    ~Player();
    PlayerUPtr static Create();
    void Init() override;
    void Update(double dt) override;
    void Render() override;
    void Shutdown() override;
private:
    Player() {};
    uint32_t LoadFile(const std::string& filename, GLenum shaderType);
    uint32_t m_vertexArrayObject { 0 };
    uint32_t m_vertexBuffer { 0 };
    uint32_t m_indexBuffer { 0 };
    uint32_t m_program { 0 };
    
};