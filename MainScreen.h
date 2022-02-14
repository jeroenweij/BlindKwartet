#pragma once
#include "drawable.h"

#include <list>
#include <memory>

class MainScreen : public Drawable
{
  public:
    MainScreen(std::list<std::string>& players);

  private:
    void Draw(Window& window) override;

    std::list<std::string>& players;
};
