#pragma once

#include "InputInterface.h"

class Input : public InputInterface
{
  public:
    int         GetInt(const int max) override;
    std::string GetString() override;

    bool CheckState(const PlayerList& players, const Cards& cards) { return true; }
};
