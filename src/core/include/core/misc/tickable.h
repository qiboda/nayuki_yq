#pragma once

class CORE_API ITickable
{
  public:
    ITickable() = default;
    virtual ~ITickable() = default;

    ITickable( const ITickable& ) = default;
    ITickable& operator=( const ITickable& ) = default;

    // Called every frame
    virtual void Tick( float deltaTime ) = 0;
};