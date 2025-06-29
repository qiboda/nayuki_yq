#pragma once

#include "meta/value/value.h"

/**
 * @brief 静态变量，或者全局变量。
 *
 */
class META_API Variable : public Value
{
  public:
    Variable();

  protected:
    // 存储值的实际数据、大小和对齐方式
    ValueStorage mValueStorage;
};
