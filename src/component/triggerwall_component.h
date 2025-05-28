#pragma once
#include "component.h"
#include "gameobject.h"
#include "model.h"
#include "program.h"
#include "scene.h"

CLASS_PTR(TriggerWallComponent)
class TriggerWallComponent : public Component {
public:
    static TriggerWallComponentPtr Create();
    ~TriggerWallComponent();
    
    void Update(float deltaTime) override;
    void Init() override;
    
    void OnTriggerEnter(GameObject* other) override;
private:
    glm::vec3  m_pendingMove;
    int       m_hasPending = 0;
    std::vector<GameObjectPtr> changeObjects;
    float minZ = 100.0f;
    float maxZ = 0.0f;
    int count = 0;
    float diff = 91.8f;

    ModelPtr modelBackpack;
    ProgramPtr programBackpack;
};