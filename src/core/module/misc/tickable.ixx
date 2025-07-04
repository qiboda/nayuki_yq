module;

#include <module_export.h>

export module core.misc.tickable;

export class CORE_API ITickable
{
  public:
    ITickable() = default;
    virtual ~ITickable() = default;

    ITickable( const ITickable& ) = default;
    ITickable& operator=( const ITickable& ) = default;

    // Called every frame
    virtual void Tick( float deltaTime ) = 0;
};