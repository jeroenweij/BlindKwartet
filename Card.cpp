#include <iostream>
#include <string>

#include "Card.h"
#include "Player.h"

Card::Card(const int id, const int kwartetId) :
    CardBase(id),
    kwartetId(kwartetId)
{
}

const bool Card::InKwartet(const int kwId) const
{
    return kwId == kwartetId;
}

Kwartet::Kwartet(const int id) :
    CardBase(id)
{
}

bool Kwartet::IsValid(const CardList& cardlist) const
{
    int cards = 0;

    for (const CardPtr& card : cardlist)
    {
        if (card->InKwartet(id))
        {
            cards++;
        }
    }
    return cards == 4;
}
