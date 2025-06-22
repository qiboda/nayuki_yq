#pragma once

#include <meta_forge/minimal.h>

struct MetaIdGenerator
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

class MetaInfo
{
  public:
    MetaInfo();

    MetaIdGenerator::MetaIdType mId = MetaIdGenerator::InvalidId;

    std::vector<std::string> mComments;
    std::vector<std::string> mToopTips;
    std::vector<std::string> mMetas;
};

class PropertyMetaInfo : public MetaInfo
{
  public:
    std::string mPropertyType;
    std::string mPropertyName;
    std::string mAccessLevel;

    bool mIsInline;
    bool mIsConst;
};

class ValueMetaInfo : public MetaInfo
{
  public:
    std::string mNamespace;
    std::string mValueType;
    std::string mValueName;
    std::string mAccessLevel;

    bool mIsInline;
    bool mIsStatic;
    bool mIsConst;
    bool mIsConstexpr;

    bool mIsRef;
    bool mIsMove;

    bool mIsInClassOrStruct;
};

class FunctionMetaInfo : public MetaInfo
{
  public:
    // 如果是类内的静态函数？则命名空间包括类名？
    std::string mNamespace;
    // 如果在类的内部
    std::string mAccessLevel;

    std::string mFunctionType;
    std::string mFunctionName;

    bool mIsInline;

    bool mIsConstexpr;
    bool mIsConsteval;
};

class MethodMetaInfo : public MetaInfo
{
  public:
    // 如果在类的内部
    std::string mAccessLevel;

    std::string mMethodType;
    std::string mMethodName;

    bool mIsConst;
    bool mIsInline;

    bool mIsRef;
    bool mIsMove;

    bool mIsConstexpr;
    bool mIsConsteval;

    bool mIsVirtual;
    bool mIsOverride;
};

struct ParentMetaInfo
{
    MetaIdGenerator::MetaIdType mId;
    std::string mAccessLevel;
};

class StructMetaInfo : public MetaInfo
{
  public:
    std::string mFromFile;
    std::string mNamespace;

    std::string mStructName;

    bool mIsConst = false;

    bool mIsFinal = false;

    std::vector<ParentMetaInfo> mParentMetaInfo;

    std::vector<FunctionMetaInfo> mFunctions;

    std::vector<MethodMetaInfo> mMethodIds;

    std::vector<PropertyMetaInfo> mProperties;
};

class ClassMetaInfo : public StructMetaInfo
{
};

struct EnumMember
{
    std::string mName;
    // reinterpret_cast 存储和提取类型。
    uint64_t mValue;
};

class EnumMetaInfo : public MetaInfo
{
  public:
    std::string mFromFile;
    std::string mNamespace;

    std::string mEnumName;

    std::string mUnderlyType;
    bool mIsSigned;

    std::vector<EnumMember> mEnumMembers;
};
