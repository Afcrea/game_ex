#pragma once
#include "component.h"

class TransformComponent : public Component {
public:
    glm::vec3 position { 0.0f, 0.0f, 0.0f };
    glm::vec3 rotation { 0.0f, 0.0f, 0.0f };
    glm::vec3 scale    { 1.0f, 1.0f, 1.0f };

    void SetPosition(const glm::vec3& pos) { position = pos; }
    glm::vec3 GetPosition() const { return position; }
    void SetRotation(const glm::vec3& rot) { rotation = rot; }
    glm::vec3 GetRotation() const { return rotation; }
    void SetScale(const glm::vec3& scl) { scale = scl; }
    glm::vec3 GetScale() const { return scale; }

    glm::mat4 GetTransformMatrix() const {
        glm::mat4 translation = glm::translate(glm::mat4(1.0f), position);
        glm::mat4 rotationX = glm::rotate(glm::mat4(1.0f), glm::radians(rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
        glm::mat4 rotationY = glm::rotate(glm::mat4(1.0f), glm::radians(rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
        glm::mat4 rotationZ = glm::rotate(glm::mat4(1.0f), glm::radians(rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
        glm::mat4 scaling = glm::scale(glm::mat4(1.0f), scale);

        return translation * rotationZ * rotationY * rotationX * scaling;
    }
};