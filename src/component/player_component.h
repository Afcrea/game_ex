#pragma once
#include "component.h"

CLASS_PTR(PlayerComponent)
class PlayerComponent : public Component {
public:
    static PlayerComponentUPtr Create();
    ~PlayerComponent();
    
    void Update(float deltaTime) override;
    void Render(CameraPtr camera) override;
    
private:
};