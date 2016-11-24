#pragma once

#include "noncopyable.hpp"

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