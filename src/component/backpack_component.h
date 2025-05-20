#pragma once
#include "component.h"

CLASS_PTR(BackPackComponent)
class BackPackComponent : public Component {
public:
    static BackPackComponentUPtr Create();
    ~BackPackComponent();
    
    void Update(float deltaTime) override;
    
    void OnTriggerEnter(GameObject* other) override;
private:
};