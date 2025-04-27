#pragma once
#include "common.h"

CLASS_PTR(Camera)
class Camera {
public:
    static CameraUPtr Create();
    void SetPerspective(float fov, float aspect, float zNear, float zFar);
    void SetView();
    void SetPosition(const glm::vec3& position);
    void SetTarget(const glm::vec3& target);
    void Configure(float fov, float aspect, float zNear, float zFar, const glm::vec3& position, const glm::vec3& up);

    const glm::mat4& GetViewMatrix() const { return m_viewMatrix; }
    const glm::mat4& GetProjectionMatrix() const { return m_projectionMatrix; }
    const glm::vec3& GetPosition() const { return m_position; }
    const glm::vec3& GetRight() const { return m_right; }
    const glm::vec3& GetFront() const { return m_front; }

    void Move(const glm::vec3& offset);
    void Rotate(float pitch, float yaw);

private:
    Camera() = default;

    glm::mat4 m_viewMatrix { 1.0f };
    glm::mat4 m_projectionMatrix { 1.0f };
    glm::vec3 m_position { 0.0f, 0.0f, 3.0f };
    glm::vec3 m_target { 1.0f, 1.0f, 1.0f };
    glm::vec3 m_front { 0.0f, 0.0f, -1.0f };
    glm::vec3 m_up { 0.0f, 1.0f, 0.0f };
    glm::vec3 m_right { 1.0f, 0.0f, 0.0f };

    float m_pitch { 0.0f };
    float m_yaw { -90.0f }; // 초기값: -Z 방향
};