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
    void          Draw(Window& window) override;
    void          DrawKwartet(Window& window, int x, int y, const int w, const int h, const Kwartet& kw);
    void          DrawCard(Window& window, const int x, int y, const int w, const int h, const Pixel& color, const CardPtr card, const int cardId, const int possiblePlId);
    const Pixel&  ColorFromState(Status state);
    const Player& PlayerFromId(const int plId) const;
    const int     GetKwartetCount(const int plId);

    const PlayerList& players;
    const Cards&      cards;
};
