#pragma once

#include <fstream>

#include "InputInterface.h"

class Input : public InputInterface
{
  public:
    Input();

    int         GetInt(const int max) override;
    std::string GetString() override;

    bool CheckState(const PlayerList& players, const Cards& cards) override;
    void Finish() override;

  private:
    std::ofstream fileOut;
};
