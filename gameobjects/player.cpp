#include "player.h"

Player::~Player() {

}

PlayerUPtr Player::Create() {
    auto player = PlayerUPtr(new Player());
    player->Init();
    return move(player);
}

void Player::Init() {
    uint32_t fs = LoadFile("gameobjects/fragment.fs", GL_FRAGMENT_SHADER);
    uint32_t vs = LoadFile("gameobjects/vertex.vs", GL_VERTEX_SHADER);

    m_program = glCreateProgram();
    
    glAttachShader(m_program, fs);
    glAttachShader(m_program, vs);

    glLinkProgram(m_program);

    int success = 0;
    glGetProgramiv(m_program, GL_LINK_STATUS, &success);

    if (!success) {
        char infoLog[1024];
        glGetProgramInfoLog(m_program, 1024, nullptr, infoLog);
        SPDLOG_ERROR("failed to link program: {}", infoLog);
    }

    float vertices[] = {
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

    unsigned int indices[] = {
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

    glGenVertexArrays(1, &m_vertexArrayObject);
    glBindVertexArray(m_vertexArrayObject);

    glGenBuffers(1, &m_vertexBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, m_vertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glGenBuffers(1, &m_indexBuffer);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_indexBuffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

    glEnable(GL_DEPTH_TEST);
}
void Player::Update(double dt) { 

}
void Player::Render() {
    glClearColor(0.1f, 0.1f, 0.15f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glUseProgram(m_program);
    glBindVertexArray(m_vertexArrayObject);
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
}
void Player::Shutdown() {

}

// 셰이더 로드
uint32_t Player::LoadFile(const std::string& filename, GLenum shaderType) {
    auto result = LoadTextFile(filename);
    if (!result.has_value()) 
        return 0;

    auto& code = result.value();
    const char* codePtr = code.c_str();
    int32_t codeLength = (int32_t)code.length();

    // create and compile shader
    uint32_t m_shader = glCreateShader(shaderType);
    glShaderSource(m_shader, 1, (const GLchar* const*)&codePtr, &codeLength);
    glCompileShader(m_shader);
    // check compile error
    int success = 0;
    glGetShaderiv(m_shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        char infoLog[1024];
        glGetShaderInfoLog(m_shader, 1024, nullptr, infoLog);
        SPDLOG_ERROR("failed to compile shader: \"{}\"", filename);
        SPDLOG_ERROR("reason: {}", infoLog);
        return 0;
    }
    return m_shader;
}