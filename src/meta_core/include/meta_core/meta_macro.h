#pragma once

/*
 * TODO: 类型别名，之类的暂时不考虑
 * VALUE: 标记全局的常量以及变量值以及各种静态变量（包括类内部的）
 * PROPERTY: 只包括非静态成员变量。
 * TOOLTIP: 悬浮提示框
 * META: 外部自定义属性
 */
#ifndef ATTR_INNER

#    define ATTR_INNER( ... ) [[clang::annotate( "NAYUKI::" #__VA_ARGS__ )]]

#    define STRUCT( ... ) ATTR_INNER( STRUCT::__VA_ARGS__ )
#    define CLASS( ... ) ATTR_INNER( CLASS::__VA_ARGS__ )
#    define ENUM( ... ) ATTR_INNER( ENUM::__VA_ARGS__ )

#    define VALUE( ... ) ATTR_INNER( VALUE::__VA_ARGS__ )

#    define PROPERTY( ... ) ATTR_INNER( PROPERTY::__VA_ARGS__ )

#    define FUNCTION( ... ) ATTR_INNER( FUNCTION::__VA_ARGS__ )
#    define METHOD( ... ) ATTR_INNER( METHOD::__VA_ARGS__ )

#    define TOOLTIP( ... ) ATTR_INNER( TOOLTIP::__VA_ARGS__ )
#    define META( ... ) ATTR_INNER( META::__VA_ARGS__ )

#endif

#ifndef ATTR_FIELD
#    define ATTR_FIELD

#    define AReadonly READONLY

#endif

#ifndef META_BODY
#    define META_BODY                                                                                                  \
      public:                                                                                                          \
        static void RegistryMeta();
#endif