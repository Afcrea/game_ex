// vertex.glsl - 정점 셰이더 (GLSL)
#version 330 core
layout(location = 0) in vec3 aPos;    // 입력 정점 위치 (attribute location 0)
uniform mat4 MVP;                    // 모델-뷰-프로젝션 행렬 (유니폼)
void main() {
    gl_Position = MVP * vec4(aPos, 1.0);
}
