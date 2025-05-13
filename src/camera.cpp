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
    m_target = position + m_front;
    m_viewMatrix = glm::lookAt(m_position, m_target, m_up);
}

void Camera::SetPerspective(float fov, float aspect, float zNear, float zFar) {
    // glm::perspective 함수 호출 수정
    m_projectionMatrix = glm::perspective(glm::radians(fov), aspect, zNear, zFar);
}

void Camera::SetView() {
    m_viewMatrix = glm::lookAt(m_position, m_position + m_front, m_up);
}

void Camera::SetTarget(const glm::vec3& target) {
    m_target = target;

    glm::vec3 direction = glm::normalize(m_target - m_position);

    glm::vec3 offset = glm::vec3(-45.0f, 60.0f, 0.0f); // 이 오프셋은 좌표계에 따라 조절
    SetPosition(target + offset);

    // pitch = 위/아래 각도, yaw = 좌/우 각도
    m_pitch = glm::degrees(asin(direction.y));                         // y축 → pitch
    m_yaw = glm::degrees(atan2(direction.z, direction.x));            // xz 평면 → yaw

    // front 벡터 재계산 (선택)
    m_front = glm::normalize(direction);
}

void Camera::SetPosition(const glm::vec3& position) {
    m_position = position;
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