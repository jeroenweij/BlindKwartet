#pragma once

#include <vector>

#include "Card.h"
#include "Kwartet.h"
#include "Player.h"

class Cards
{
  public:
    Cards(const int numCards, const PlayerList& players);

    bool CheckInit(const int numCards);

    const KwartetList& GetKwartets() const;
    KwartetList&       GetKwartetsMutable();
    const CardList&    GetCards() const;
    CardList&          GetCardsMutable();

  private:
    CardList    cards;
    KwartetList kwartets;
};
