#pragma once

#include "core/container/small_vector.h"
#include "core/helper/string.h"
#include <core/minimal.h>
#include <meta/minimal.h>
#include <fmt/xchar.h>

class META_API Namespace
{
  public:
    Namespace();

    Namespace( const std::string& name )
        : mName( name ) {};

    static std::unique_ptr<Namespace> Parse( const std::string& name )
    {
        auto parts = StringHelper::Split<8>( name, "::" );
        std::unique_ptr<Namespace> ns = nullptr;
        if ( parts.size() > 0 )
        {
            ns = std::make_unique<Namespace>( parts[0] );
        }
        std::unique_ptr<Namespace>* last_ns = &ns;
        for ( usize i = 1; i < parts.size(); ++i )
        {
            std::unique_ptr<Namespace> inner_ns = std::make_unique<Namespace>( parts[i] );
            ( *last_ns )->Append( std::move( inner_ns ) );
            last_ns = &inner_ns;
        }
        return ns;
    }

    std::string GetName()
    {
        return mName;
    }

    std::string GetFullName()
    {
        if ( mNamespace )
        {
            return mName + "::" + mNamespace->GetFullName();
        }
        return mName;
    }

    void Append( std::unique_ptr<Namespace> ns )
    {
        mNamespace = std::move( ns );
    }

  protected:
    std::unique_ptr<Namespace> mNamespace;
    std::string mName;
};