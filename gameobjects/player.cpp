#include "player.h"
#include "scene_manager.h"
#include "main_scene.h"

Player::~Player() {
    SPDLOG_INFO("player shutdown");
}

PlayerUPtr Player::Create() {
    auto player = PlayerUPtr(new Player());
    player->Init();

    return move(player);
}

void Player::Configure() {
    playerVAO = VertexLayout::Create();

    playerVBO = Buffer::CreateWithData(GL_ARRAY_BUFFER, GL_STATIC_DRAW, GetVertices().data(), sizeof(float), GetVertices().size());
    playerIBO = Buffer::CreateWithData(GL_ELEMENT_ARRAY_BUFFER, GL_STATIC_DRAW, GetIndices().data(), sizeof(uint32_t), GetIndices().size());
    
    playerVAO->SetAttrib(0, 3, GL_FLOAT, false, sizeof(float) * 3, 0);
    glEnable(GL_DEPTH_TEST);
}

void Player::Init() {
    fs = Shader::CreateFromFile("gameobjects/fragment.fs", GL_FRAGMENT_SHADER);
    vs = Shader::CreateFromFile("gameobjects/vertex.vs", GL_VERTEX_SHADER);

    vertices = {
        // front face (z =  1.0)
        -1.0f, -1.0f,  1.0f,   // v0
         1.0f, -1.0f,  1.0f,   // v1
         1.0f,  1.0f,  1.0f,   // v2
        -1.0f,  1.0f,  1.0f,   // v3
        // back face (z = -1.0)
        -1.0f, -1.0f, -1.0f,   // v4
         1.0f, -1.0f, -1.0f,   // v5
         1.0f,  1.0f, -1.0f,   // v6
        -1.0f,  1.0f, -1.0f    // v7
    };

    indices = {
        // front face
        0, 1, 2,   2, 3, 0,
        // right face
        1, 5, 6,   6, 2, 1,
        // back face
        7, 6, 5,   5, 4, 7,
        // left face
        4, 0, 3,   3, 7, 4,
        // bottom face
        4, 5, 1,   1, 0, 4,
        // top face
        3, 2, 6,   6, 7, 3
    };
}
void Player::Update(double dt) { 

}
void Player::Render() {
    glClearColor(0.1f, 0.1f, 0.15f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glUseProgram(programID);
    playerVAO->Bind();
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
}
void Player::Shutdown() {

}