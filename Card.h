#pragma once

#include <list>
#include <memory>
#include <string>

#include "CardBase.h"

class Player;

class Card;
using CardPtr  = std::shared_ptr<Card>;
using CardList = std::vector<CardPtr>;

class Card : public CardBase
{
  public:
    Card(const int id, const int kwartetId);
    const bool InKwartet(const int kwId) const;
    void       NotOwnedBy(const int playerId);

    bool Check(const std::vector<Player>& players);

  private:
    const int      kwartetId;
    std::list<int> notOwnedBy;
};
