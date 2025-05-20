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
    glm::vec3 GetForward() const {
        glm::vec3 forward = glm::vec3(0.0f, 0.0f, 1.0f);
        glm::quat q = glm::quat(glm::radians(rotation));
        return q * forward;
    }

    glm::mat4 GetTransformMatrix() const {
        if (useWorldMatrix) return worldMatrix;
        return CalcLocalMatrix();
    }

    void SetTransformMatrix(const glm::mat4& mat) {
        position = glm::vec3(mat[3]);

        // 2) 스케일 추출 (각 축 벡터의 길이)
        glm::vec3 col0 = glm::vec3(mat[0]);
        glm::vec3 col1 = glm::vec3(mat[1]);
        glm::vec3 col2 = glm::vec3(mat[2]);
        scale.x = glm::length(col0);
        scale.y = glm::length(col1);
        scale.z = glm::length(col2);

        // 3) 회전 행렬(3×3) 추출 후 쿼터니언 변환
        glm::mat3 rotMat;
        rotMat[0] = col0 / scale.x;
        rotMat[1] = col1 / scale.y;
        rotMat[2] = col2 / scale.z;
        glm::quat orient = glm::quat_cast(rotMat);

        // 4) Euler 각(도 단위)로 변환
        rotation = glm::degrees(glm::eulerAngles(orient));
    }

    void SetWorldTransformMatrix(const glm::mat4& mat) {
        worldMatrix     = mat;
        useWorldMatrix  = true;
    }

private:
    bool     useWorldMatrix = false;
    glm::mat4 worldMatrix   = glm::mat4(1.0f);
    // 로컬 매트릭스 계산
    glm::mat4 CalcLocalMatrix() const {
        glm::mat4 T = glm::translate(glm::mat4(1.0f), position);
        glm::mat4 Rx= glm::rotate(glm::mat4(1.0f), glm::radians(rotation.x), {1,0,0});
        glm::mat4 Ry= glm::rotate(glm::mat4(1.0f), glm::radians(rotation.y), {0,1,0});
        glm::mat4 Rz= glm::rotate(glm::mat4(1.0f), glm::radians(rotation.z), {0,0,1});
        glm::mat4 S = glm::scale   (glm::mat4(1.0f), scale);
        return T * Rz * Ry * Rx * S;
    }
};