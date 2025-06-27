#pragma once

#include "meta/value/value.h"
#include <core/minimal.h>
#include <meta/minimal.h>

class META_API Function : public Value
{
  public:
    template <typename TFunc>
    Function( std::string& name, TFunc* func )
        : mFunc( func )
        , mName( name )
    {
    }

    template <usize TIndex, typename TParam>
    void Param( std::string& name )
    {
        if ( mParamName.size() < TIndex )
        {
            mParamName.resize( TIndex );
        }
        mParamName[TIndex] = name;
    }

    void Invoke()
    {
    }

  protected:
    void* mFunc;
    std::string mName;

    std::vector<std::string> mParamName;
};