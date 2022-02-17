#pragma once

#include "Card.h"

#include <string>
#include <vector>

class Player;
using PlayerList = std::vector<Player>;

class Player
{
  public:
    Player(std::string name, const int numCards);

    const std::string Name() const;
    const int         GetId() const;

    const int CardsInHand() const;
    const int CardsUnclaimed() const;
    void      ClaimedCards(const int numCards);
    void      LostCard();
    void      AddCard();
    void      ClaimCard(CardPtr card);

  private:
    const int         id;
    const std::string name;
    int               unclaimedCards;
    int               claimedCards;
};
