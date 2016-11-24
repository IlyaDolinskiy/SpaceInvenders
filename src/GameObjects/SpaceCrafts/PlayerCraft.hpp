#pragma once

#include "ISpaceCrafts.hpp"
#include "../../Patterns/Singleton.hpp"

class PlayerCraft : public ISpaceCrafts, protected Singleton <PlayerCraft>
{
public:
  virtual ~PlayerCraft() {};

  void do_smth();
protected:
  friend class Singleton<PlayerCraft>;

public:
  int m_i;
};

#define Player Singleton<PlayerCraft>::Instance()