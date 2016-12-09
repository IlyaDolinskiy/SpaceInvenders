#pragma once

#include "../ISpaceObjects.hpp"

class ISpaceCrafts : public ISpaceObjects
{
public:
  virtual ~ISpaceCrafts() {};
  virtual GameObjectsTypes GetType() const override = 0;
};