#pragma once
#include "component.h"

CLASS_PTR(PlayerComponent)
class PlayerComponent : public Component {
public:
    static PlayerComponentPtr Create();
    ~PlayerComponent();
    
    void Update(float deltaTime) override;
    void Render(CameraPtr camera) override;
    
    void OnTriggerEnter(GameObject* other) override;
private:
    float m_speed = 10.0f;
    int m_score = 0;
};