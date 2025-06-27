#pragma once

#include "meta/type_base.h"
#include <core/minimal.h>
#include <meta/minimal.h>

struct META_API ValueStorage
{
    // 存储值的实际数据
    void* mData;
    // 存储值的大小
    size_t mSize;
    // 存储值的对齐方式
    size_t mAlignment;
};

/**
 * @brief
 * --------------
 * 全局变量(包括命名空间内), 需要存储空间(存储地址)
 * 类或者结构体内的静态变量, 需要存储空间(存储地址)
 * 函数指针, 需要存储空间(存储地址)
 * 成员函数指针, 需要存储空间(存储地址)

 * 成员变量。(不需要存储空间)，所以成员变量不是值类型。
 */

/**
 * @brief
 * 值类型: 包括函数, 变量等。
 */
class META_API Value
{
  public:
    Value();

  public:
    // 获取类型ID
    TypeId GetTypeId() const;

    // 获取名字
    const std::string& GetName() const;

    // 设置值
    void SetValue( const Value& value );

    // 获取值
    const Value& GetValue() const;

  protected:
    TypeId mTypeId;    // 值的类型ID
    std::string mName; // 值的名字
};