#pragma once
#include "Cards.h"
#include "Player.h"
#include "colors.h"
#include "drawable.h"

#include <list>
#include <memory>

class MainScreen : public Drawable
{
  public:
    MainScreen(const PlayerList& players, const Cards& cards);

  private:
    void         Draw(Window& window) override;
    void         DrawKwartet(Window& window, int x, int y, const int w, const int h, const Kwartet& kw);
    void         DrawCard(Window& window, const int x, int y, const int w, const int h, const CardPtr card);
    const Pixel& ColorFromState(Status state);

    const PlayerList& players;
    const Cards&      cards;
};
