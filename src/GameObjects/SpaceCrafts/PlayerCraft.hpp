#pragma once

#include "ISpaceCrafts.hpp"

class PlayerCraft : public ISpaceCrafts
{
public:
  virtual ~PlayerCraft() {};

  void do_smth();
public:
  int m_i;
};