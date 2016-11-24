#pragma once

#include "ISpaceCrafts.hpp"

class AlienCraft : public ISpaceCrafts
{
public:
  ~AlienCraft() override {};
  GameObjectsTypes GetType() const override { return GameObjectsTypes::AlienCraft; }
};