#include "Box.h"

Box::~Box() {
    SPDLOG_INFO("Box shutdown");
}

BoxUPtr Box::Create() {
    auto box = BoxUPtr(new Box());
    box->Init();
    return std::move(box);
}

void Box::Init() {
    // 셰이더 로드
    fs = Shader::CreateFromFile("gameobjects/Box.fs", GL_FRAGMENT_SHADER);
    vs = Shader::CreateFromFile("gameobjects/Box.vs", GL_VERTEX_SHADER);

    // 정점 데이터 (포지션 + UV)
    vertices = {
        // pos               // tex
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
        0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
        0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,

        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
        0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 1.0f
    };

    indices = {
        0, 1, 2, 2, 3, 0,  // back
        4, 5, 6, 6, 7, 4,  // front
        4, 0, 3, 3, 7, 4,  // left
        1, 5, 6, 6, 2, 1,  // right
        3, 2, 6, 6, 7, 3,  // top
        4, 5, 1, 1, 0, 4   // bottom

    };
}

void Box::Configure() {
    vao = VertexLayout::Create();
    vbo = Buffer::CreateWithData(GL_ARRAY_BUFFER, GL_STATIC_DRAW, vertices.data(), sizeof(float), vertices.size());
    ibo = Buffer::CreateWithData(GL_ELEMENT_ARRAY_BUFFER, GL_STATIC_DRAW, indices.data(), sizeof(uint32_t), indices.size());

    vao->SetAttrib(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 5, 0); // position
    vao->SetAttrib(1, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 5, sizeof(float) * 3); // UV

    texture = Texture::CreateFromImage(Image::Load("gameobjects/container2.png").get());

    glEnable(GL_DEPTH_TEST);
}
void Box::Update(double dt) { 

}

void Box::Render() {
    glUseProgram(programID);
    glActiveTexture(GL_TEXTURE0);
    texture->Bind();
    vao->Bind();
    GLint loc = glGetUniformLocation(programID, "uTexture");

    glUniform1i(loc, 0);            // 텍스처 유니폼에 슬롯 인덱스 전달
    glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(indices.size()), GL_UNSIGNED_INT, 0);
}

void Box::Shutdown() {
    // 리소스 해제
}