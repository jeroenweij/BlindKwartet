#pragma once

#include "windowStack.h"

#include <list>
#include <string>

class Kwartetter
{
  public:
    Kwartetter(std::list<std::string> players, const int numCards);
    ~Kwartetter();

  private:
    WindowStack windows;
};
