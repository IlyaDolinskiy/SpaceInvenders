#pragma once

#include "ISpaceCrafts.hpp"
#include "../../Patterns/Singleton.hpp"

class PlayerCraft : public ISpaceCrafts, protected patterns::Singleton <PlayerCraft>
{
public:
  ~PlayerCraft() override {};
  GameObjectsTypes GetType() const override { return GameObjectsTypes::Player; }

  void do_smth();
protected:
  friend class Singleton<PlayerCraft>;

public:
  int m_i;
};

#define Player patterns::Singleton<PlayerCraft>::Instance()