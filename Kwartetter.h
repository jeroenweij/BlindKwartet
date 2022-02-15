#pragma once

#include "Cards.h"
#include "Player.h"
#include "windowStack.h"

#include <list>
#include <string>

class Kwartetter
{
  public:
    Kwartetter(PlayerList& players, const int numCards);
    ~Kwartetter();

    bool CheckInit(const int numCards) const;
    void Start();

  private:
    bool        Ask(std::string question);
    std::string AskString(std::string question);
    Player&     AskUser(std::string question);
    bool        HasAnywoneKwartet();
    Kwartet&    AskKwartet();
    CardPtr     AskKwartetCard(const int kwId);

    const bool UnclaimedKwartetAvailable() const;
    Kwartet&   UnclaimedKwartetGet();
    Kwartet*   UnNamedKwartetGet();
    CardPtr    UnNamedCardGet(const int kwId);

    Player& GetNextPlayer();

    bool CheckPlayerHasCardInSeries(Player& pl, Kwartet& kw);
    bool CheckPlayer2AnswerYes(Player& pl1, Player& pl2, const CardPtr card);

    bool GameIsRunning();
    bool GameComplete();
    void GameOver(std::string reason);

    PlayerList& players;
    Cards       cards;
    WindowStack windows;
    bool        stop;
};
