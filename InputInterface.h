#pragma once

#include <string>

#include "Cards.h"
#include "Player.h"

class InputInterface
{
  public:
    virtual int         GetInt(const int max) = 0;
    virtual std::string GetString()           = 0;

    virtual bool CheckState(const PlayerList& players, const Cards& cards) = 0;
    virtual void Finish()                                                  = 0;
};
