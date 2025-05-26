#pragma once
#include "common.h"

struct Light {
        glm::vec3 position { glm::vec3(0.0f, 10.0f, 0.0f) };
        glm::vec3 direction { glm::vec3(-90.0f, 90.0f, 0.0f) };
        glm::vec2 cutoff { glm::vec2(20.0f, 5.0f) };
        float distance { 32.0f };
        glm::vec3 ambient { glm::vec3(0.1f, 0.1f, 0.1f) };
        glm::vec3 diffuse { glm::vec3(0.5f, 0.5f, 0.5f) };
        glm::vec3 specular { glm::vec3(1.0f, 1.0f, 1.0f) };
};

CLASS_PTR(Camera)
class Camera {
public:
    static CameraUPtr Create();
    void SetPerspective(float fov, float aspect, float zNear, float zFar);
    void SetView();
    void SetPosition(const glm::vec3& position) { m_position = position; }
    void SetTarget(const glm::vec3& target) { m_target = target; }
    void SetAspect(float aspect) { SetPerspective(m_fov, aspect, m_zNear, m_zFar); }
    void Configure(float fov, float aspect, float zNear, float zFar, const glm::vec3& position, const glm::vec3& up);

    const glm::mat4& GetViewMatrix() const { return m_viewMatrix; }
    const glm::mat4& GetProjectionMatrix() const { return m_projectionMatrix; }
    const glm::vec3& GetPosition() const { return m_position; }
    const glm::vec3& GetRight() const { return m_right; }
    const glm::vec3& GetFront() const { return m_front; }
    const glm::vec3& GetTarget() const { return m_target; }
    
    void Move(const glm::vec3& offset);
    void Rotate(float pitch, float yaw);

    void Follow(
        const glm::vec3& targetPosition,
        const glm::vec3& targetForward,
        float distance = 10.0f,
        float height   =  5.0f
    );

    void SetLight(const Light& light) { m_light = light; }
    const Light& GetLight() const { return m_light; }
private:
    Camera() = default;

    glm::mat4 m_viewMatrix { 1.0f };
    glm::mat4 m_projectionMatrix { 1.0f };
    glm::vec3 m_position { 0.0f, 0.0f, 3.0f };
    glm::vec3 m_target { 1.0f, 1.0f, 1.0f };
    glm::vec3 m_front { 0.0f, 0.0f, -1.0f };
    glm::vec3 m_up { 0.0f, 1.0f, 0.0f };
    glm::vec3 m_right { 1.0f, 0.0f, 0.0f };
    float m_fov { 1.0f };
    float m_aspect { 1.0f };
    float m_zNear { 0.1f };
    float m_zFar { 10.0f };

    float m_pitch { 0.0f };
    float m_yaw { -90.0f }; // 초기값: -Z 방향

    Light m_light;
};