#pragma once

#ifndef NONCOPYABLE_H
#define NONCOPYABLE_H

class NonCopyable {
protected:
  NonCopyable() {}
  ~NonCopyable() {}

private:
  NonCopyable &operator=(const NonCopyable &) = delete;
  NonCopyable(const NonCopyable &) = delete;
};

#endif // !NONCOPYABLE_H
