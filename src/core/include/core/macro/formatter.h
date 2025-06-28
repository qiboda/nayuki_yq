#pragma once

#ifndef NAME_FORMATTER
#    define NAME_FORMATTER( Type )                                                                                     \
        template <>                                                                                                    \
        struct fmt::formatter<Type>                                                                                    \
        {                                                                                                              \
            constexpr auto parse( fmt::format_parse_context& ctx )                                                     \
            {                                                                                                          \
                return ctx.begin();                                                                                    \
            }                                                                                                          \
                                                                                                                       \
            template <typename TFormatContext>                                                                         \
            auto format( const Type& v, TFormatContext& ctx ) const                                                    \
            {                                                                                                          \
                UNUSED_VARS( v );                                                                                      \
                UNUSED_VARS( ctx );                                                                                    \
                return fmt::format_to( ctx.out(), #Type );                                                             \
            }                                                                                                          \
        };
#endif