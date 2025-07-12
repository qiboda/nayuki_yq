module;

#include <core/logger/category.h>
#include <module_export.h>
#include <meta_core/meta_macro.h>

export module asset;

import core.logger.category;
import core;

export ASSET_API NY_LOG_CATEGORY( LogAsset );

class CLASS( a class a ) A
{
  public:
    PROPERTY( alksjdf )
    i32 k;
};