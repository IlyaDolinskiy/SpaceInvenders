#pragma once

#include "noncopyable.hpp"

namespace patterns
{
template <class T>
class Singleton: private noncopyable
{
public:
  static T& Instance()
  {
    static T instance;
    return instance;
  }
};
}