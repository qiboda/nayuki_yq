#pragma once

class IRAII {
  public:

    IRAII() = default;
    virtual ~IRAII() = default;

    IRAII(const IRAII &) = default;
    IRAII &operator=(const IRAII &) = default;

    virtual void Initialize() = 0;

    virtual void CleanUp() = 0;
};