#pragma once

#include <list>
#include <memory>
#include <string>

#include "CardBase.h"

class Player;

class Card;
using CardPtr  = std::shared_ptr<Card>;
using CardList = std::vector<CardPtr>;

class Kwartet;
using KwartetList = std::vector<Kwartet>;

class Card : public CardBase
{
  public:
    Card(const int id, const int kwartetId);
    const bool            InKwartet(const int kwId) const;
    void                  NotOwnedBy(const int playerId);
    const std::list<int>& GetNotOwnedByPlayers() const;

    bool Check(std::vector<Player>& players, KwartetList& kwartets);

  private:
    const int      kwartetId;
    std::list<int> notOwnedBy;
};
