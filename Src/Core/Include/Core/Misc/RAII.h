#pragma once

class IRAII {
  public:
    virtual void Initialize() = 0;

    virtual void CleanUp() = 0;
};