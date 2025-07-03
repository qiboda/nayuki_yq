#pragma once

import core;

enum class ContextPathType : u8
{
    Global = 0,
    Class = 1 << 0,
    Struct = 1 << 1,
    Namespace = 1 << 2,

    All = Global | Class | Struct | Namespace,
};

// The bitwise OR operator must be defined - this is how reflect-cpp knows that
// this is a flag enum.
inline ContextPathType operator|( ContextPathType c1, ContextPathType c2 )
{
    return static_cast<ContextPathType>( static_cast<int>( c1 ) | static_cast<int>( c2 ) );
}

template <>
struct std::hash<ContextPathType>
{
    auto operator()( const ContextPathType& key ) const
    {
        std::size_t seed;
        hash_combine( seed, static_cast<uint8_t>( key ) );
        return seed;
    }
};

inline bool Has( ContextPathType self, ContextPathType type )
{
    return ( static_cast<uint8_t>( self ) & static_cast<uint8_t>( type ) ) != 0;
}

struct META_FORGE_API ContextPath
{
    std::string mName;
    ContextPathType mType;

    friend bool operator==( const ContextPath& lhs, const ContextPath& rhs )
    {
        return lhs.mName == rhs.mName && lhs.mType == rhs.mType;
    }
};

template <>
struct std::hash<ContextPath>
{
    auto operator()( const ContextPath& key ) const
    {
        std::size_t seed;
        hash_combine( seed, key.mName, key.mType );
        return seed;
    }
};

struct META_FORGE_API ContextCombinedPath
{
    std::vector<ContextPath> mCombinedPath;

    std::string GetCombinedPathString() const
    {
        std::string path;
        for ( auto it = mCombinedPath.begin(); it != mCombinedPath.end(); ++it )
        {
            path += it->mName;
            if ( it + 1 != mCombinedPath.end() )
                path += "::";
        }
        return path;
    }

    friend bool operator==( const ContextCombinedPath& lhs, const ContextCombinedPath& rhs )
    {
        return lhs.mCombinedPath == rhs.mCombinedPath;
    }
};

template <>
struct std::hash<ContextCombinedPath>
{
    auto operator()( const ContextCombinedPath& key ) const
    {
        std::size_t seed = 0u;
        for ( auto it = key.mCombinedPath.begin(); it != key.mCombinedPath.end(); ++it )
        {
            hash_combine( seed, *it );
        }
        return seed;
    }
};

enum class TypeQualifier : uint32_t
{
    None = 0,
    Volatile = 1 << 0,
    Const = 1 << 1,

    Static = 1 << 2,
    Inline = 1 << 3,
    IsConstexpr = 1 << 4,
};

enum class VariableType : uint8_t
{
    None = 0,
    LVRef = 1,
    RVRef = 2,
    Ptr = 3,
};

struct META_FORGE_API MetaIdGenerator
{
    using MetaIdType = uint32_t;

    /**
     * 0 是无效Id。
     */
    static MetaIdType Next()
    {
        sId += 1;
        return sId;
    }

    static inline MetaIdType sId = 0;
    static inline MetaIdType InvalidId = 0;
};

struct META_FORGE_API MetaInfo
{
  public:
    MetaIdGenerator::MetaIdType mId = MetaIdGenerator::InvalidId;

    std::string mComment;
    std::vector<std::string> mToopTips;
    std::vector<std::string> mMetas;
};

struct META_FORGE_API PropertyMetaInfo
{
  public:
    MetaInfo mMetaInfo;

    std::string mPropertyType;
    std::string mPropertyName;
    std::string mAccessLevel;

    bool mIsVolatile;
    bool mIsConst;
    bool mIsLVRef;
    bool mIsRVRef;
    bool mIsPtr;
};

/**
 * @brief
 * 全局变量或者命名空间下的。包括 class 或者 struct 内部的。
 *
 * 是外部链接的类型才支持(链接器保证全局唯一)，而且如果需要跨dll，必须导出(如果不导出，则每个dll有一个独立的全局变量）
 * TODO: 多处包含，如何排除？
 */
struct META_FORGE_API ValueMetaInfo
{
  public:
    MetaInfo mMetaInfo;

    std::string mFilePath;
    std::string mContextPath;
    std::string mValueType;
    std::string mValueName;
    std::string mAccessLevel;

    bool mIsInline;
    bool mIsStatic;
    bool mIsConstexpr;

    bool mIsVolatile;
    bool mIsConst;
    bool mIsLVRef;
    bool mIsRVRef;
    bool mIsPtr;
};

struct META_FORGE_API FunctionParam
{
    std::string mName;
};

struct META_FORGE_API FunctionMetaInfo
{
  public:
    MetaInfo mMetaInfo;

    // 如果是类内的静态函数？则命名空间包括类名？
    std::string mContextPath;
    // 如果在类的内部
    std::string mAccessLevel;

    std::string mFunctionType;
    std::string mFunctionName;
    std::vector<FunctionParam> mFunctionParams;

    bool mIsInline;

    bool mIsConstexpr;
    bool mIsConsteval;
};

struct META_FORGE_API MethodParam
{
    std::string mName;
};

struct META_FORGE_API MethodMetaInfo
{
  public:
    MetaInfo mMetaInfo;

    // 如果在类的内部
    std::string mAccessLevel;

    std::string mMethodType;
    std::string mMethodName;
    std::vector<MethodParam> mMethodParams;

    bool mIsConst;
    bool mIsInline;

    bool mIsRef;
    bool mIsMove;

    bool mIsConstexpr;
    bool mIsConsteval;

    bool mIsVirtual;
    bool mIsOverride;
};

struct META_FORGE_API ParentMetaInfo
{
    MetaIdGenerator::MetaIdType mId;
    std::string mAccessLevel;
};

struct META_FORGE_API StructMetaInfo
{
  public:
    MetaInfo mMetaInfo;

    std::string mFilePath;
    std::string mContextPath;

    std::string mStructName;

    bool mIsFinal = false;

    std::vector<ParentMetaInfo> mParentMetaInfo;

    std::vector<FunctionMetaInfo> mFunctions;

    std::vector<MethodMetaInfo> mMethods;

    std::vector<PropertyMetaInfo> mProperties;

    std::vector<ValueMetaInfo> mValues;
};

struct META_FORGE_API ClassMetaInfo
{
  public:
    MetaInfo mMetaInfo;

    std::string mFilePath;
    std::string mContextPath;

    std::string mClassName;

    bool mIsFinal = false;

    std::vector<ParentMetaInfo> mParentMetaInfo;

    std::vector<FunctionMetaInfo> mFunctions;

    std::vector<MethodMetaInfo> mMethods;

    std::vector<PropertyMetaInfo> mProperties;

    std::vector<ValueMetaInfo> mValues;
};

struct META_FORGE_API EnumMember
{
    std::string mName;
    // reinterpret_cast 存储和提取类型。
    uint64_t mValue;
};

struct META_FORGE_API EnumMetaInfo
{
  public:
    MetaInfo mMetaInfo;

    std::string mFilePath;
    std::string mContextPath;

    std::string mEnumName;

    std::string mUnderlyingType;
    bool mIsSigned;

    std::vector<EnumMember> mEnumMembers;
};

struct META_FORGE_API MetaInfoUniqueKey
{
    std::string mFilePath;
    ContextCombinedPath mContextPath;
    std::string mVariableName;

    friend bool operator==( const MetaInfoUniqueKey& left, const MetaInfoUniqueKey& right ) = default;
};

template <>
struct std::hash<MetaInfoUniqueKey>
{
    auto operator()( const MetaInfoUniqueKey& key ) const
    {
        std::size_t seed;
        hash_combine( seed, key.mFilePath, key.mContextPath, key.mVariableName );
        return seed;
    }
};

/**
 * @brief
 * 还缺少union和c style enum？
 */
struct META_FORGE_API MetaInfoManager
{
  public:
    std::unordered_map<MetaInfoUniqueKey, ClassMetaInfo> mAllClassMetaInfo;
    std::unordered_map<MetaInfoUniqueKey, StructMetaInfo> mAllStructMetaInfo;
    std::unordered_map<MetaInfoUniqueKey, EnumMetaInfo> mAllEnumMetaInfo;
    // 只包含全局变量和命名空间中的。
    std::unordered_map<MetaInfoUniqueKey, ValueMetaInfo> mAllValueMetaInfo;
};