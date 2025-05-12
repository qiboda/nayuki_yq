#pragma once

#include <core/delegate/delegate.h>

#define DECL_MULTI_DElEGATE_TYPE( DelegateType, ... ) typedef MultipleDelegate<void, __VA_ARGS__> DelegateType

#define DECL_SINGLE_DElEGATE_TYPE( DelegateType, ... ) typedef SingleDelegate<void, __VA_ARGS__> DelegateType
#define DECL_SINGLE_DElEGATE_TYPE_RETVAL( DelegateType, ... ) typedef SingleDelegate<__VA_ARGS__> DelegateType