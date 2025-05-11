#pragma once

class NonCopyable
{
  protected:
    NonCopyable()
    {
    }

    virtual ~NonCopyable()
    {
    }

  private:
    NonCopyable &operator=( const NonCopyable & ) = delete;
    NonCopyable( const NonCopyable & ) = delete;
};
