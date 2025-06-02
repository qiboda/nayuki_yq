#pragma once

#ifndef NAME_FORMATTER
#    define NAME_FORMATTER( Type )                                                                                     \
        template <>                                                                                                    \
        struct std::formatter<Type>                                                                                    \
        {                                                                                                              \
            constexpr auto parse( std::format_parse_context& ctx )                                                     \
            {                                                                                                          \
                return ctx.begin();                                                                                    \
            }                                                                                                          \
                                                                                                                       \
            template <typename TFormatContext>                                                                         \
            auto format( const Type& v, TFormatContext& ctx ) const                                                    \
            {                                                                                                          \
                UNUSED_VAR( v );                                                                                       \
                UNUSED_VAR( ctx );                                                                                     \
                return std::format_to( ctx.out(), #Type );                                                             \
            }                                                                                                          \
        };
#endif