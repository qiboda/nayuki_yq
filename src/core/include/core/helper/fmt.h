
#include "core/type_def.h"
#include <fmt/base.h>
#include <iomanip>
#include <sstream>

namespace FmtHelper
{

template <typename TClass, typename TRet, typename... TArgs>
inline std::string MemberFnPtr( TRet ( TClass::*ptr )( TArgs... ) )
{
    unsigned char buf[sizeof( ptr )];
    memcpy( buf, &ptr, sizeof( ptr ) );
    std::ostringstream oss;
    oss << "0x";
    for ( size_t i = 0; i < sizeof( ptr ); ++i )
        oss << std::hex << std::setw( 2 ) << std::setfill( '0' ) << static_cast<i32>( buf[i] );
    return oss.str();
}

} // namespace FmtHelper