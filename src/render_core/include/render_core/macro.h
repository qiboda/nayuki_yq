#pragma once

#ifndef RENDER_TYPE_WRAPPER
#    define RENDER_TYPE_WRAPPER( Type )                                                                                \
        export using Shared##Type = std::shared_ptr<Type>;                                                             \
        export using Weak##Type = std::weak_ptr<Type>;                                                                 \
        export using Unique##Type = std::unique_ptr<Type>;
#endif // RENDER_TYPE_WRAPPER