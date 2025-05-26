#include "camera.h"

CameraUPtr Camera::Create() {
    return CameraUPtr(new Camera());
}

void Camera::Configure(float fov, float aspect, float zNear, float zFar, const glm::vec3& position, const glm::vec3& up) {
    SetPerspective(fov, aspect, zNear, zFar);
    SetPosition(position);
    m_front = glm::rotate(glm::mat4(1.0f), glm::radians(m_yaw), glm::vec3(0.0f, 1.0f, 0.0f)) *
                        glm::rotate(glm::mat4(1.0f), glm::radians(m_pitch), glm::vec3(1.0f, 0.0f, 0.0f)) *
                        glm::vec4(0.0f, 0.0f, -1.0f, 0.0f);
    m_right = glm::normalize(glm::cross(m_front, up));
    m_up = glm::cross(m_right, m_front);
    SetTarget(m_position + m_front);
    m_viewMatrix = glm::lookAt(m_position, m_target, m_up);
}

void Camera::SetPerspective(float fov, float aspect, float zNear, float zFar) {
    // glm::perspective 함수 호출 수정
    m_fov = fov;
    m_aspect = aspect;
    m_zNear = zNear;
    m_zFar = zFar;
    m_projectionMatrix = glm::perspective(glm::radians(m_fov), m_aspect, m_zNear, m_zFar);
}

void Camera::SetView() {
    m_viewMatrix = glm::lookAt(m_position, m_target, m_up);
}

void Camera::Move(const glm::vec3& offset) {
    m_position += offset;
    SetPosition(m_position);
}

void Camera::Rotate(float pitch, float yaw) {
    m_pitch += pitch;
    m_yaw += yaw;

    // 제한 각도 설정 (예: -89도 ~ 89도)
    m_pitch = glm::clamp(m_pitch, -89.0f, 89.0f);

    glm::vec3 front;
    front.x = cos(glm::radians(m_yaw)) * cos(glm::radians(m_pitch));
    front.y = sin(glm::radians(m_pitch));
    front.z = sin(glm::radians(m_yaw)) * cos(glm::radians(m_pitch));
    m_front = glm::normalize(front);

    // 업데이트
    m_right = glm::normalize(glm::cross(m_front, glm::vec3(0.0f, 1.0f, 0.0f)));
    m_up = glm::cross(m_right, m_front);
    SetView();
}

void Camera::Follow(
    const glm::vec3& targetPos,
    const glm::vec3& targetFwd,
    float distance,
    float height
) {
    SetTarget(targetPos);
    // 1) 전방 벡터 평면 투영 및 정규화
    glm::vec3 flatFwd = glm::normalize(glm::vec3(targetFwd.x, 0.0f, targetFwd.z));
    if (glm::length(flatFwd) < 0.001f)
        flatFwd = glm::vec3(0.0f, 0.0f, -1.0f);

    // 2) 카메라 위치 계산: 뒤로 distance 만큼, 위로 height 만큼
    m_position = targetPos - flatFwd * distance + glm::vec3(0.0f, height, 0.0f);

    // 3) 앞(front), 오른쪽(right), 위(up) 벡터 갱신
    m_front = glm::normalize(targetPos - m_position);
    glm::vec3 worldUp(0.0f, 1.0f, 0.0f);
    m_right = glm::normalize(glm::cross(m_front, worldUp));
    m_up    = glm::cross(m_right, m_front);
}

