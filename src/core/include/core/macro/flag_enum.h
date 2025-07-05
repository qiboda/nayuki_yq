#pragma once

#ifndef FLAG_ENUM
#    define FLAG_ENUM( E )                                                                                             \
        template <>                                                                                                    \
        struct IsFlagEnum<E> : std::true_type                                                                          \
        {                                                                                                              \
        };
#endif
