#pragma once

#include "core/misc/non_copyable.h"

class IRAII : public NonCopyable
{
  public:
    virtual ~IRAII() = default;

    virtual void Initialize() = 0;

    virtual void CleanUp() = 0;
};