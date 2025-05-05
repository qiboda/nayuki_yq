#pragma once

class ITickable {

  public:
    // Called every frame
    virtual void Tick(float deltaTime) = 0;
};