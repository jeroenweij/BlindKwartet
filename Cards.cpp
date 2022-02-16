#include "Cards.h"

Cards::Cards(const int numCards, const PlayerList& players)
{
    int k = 0;
    kwartets.push_back({k});

    for (int i = 0; i < numCards; i++)
    {
        if (i > 0 && i % 4 == 0)
        {
            k++;
            kwartets.push_back({k});
        }
        cards.push_back(std::make_shared<Card>(i, kwartets.back().GetId()));
    }
}

bool Cards::CheckInit(const int numCards) const
{
    if (cards.size() != numCards)
    {
        return false;
    }
    if (kwartets.size() != numCards / 4)
    {
        return false;
    }

    for (const Kwartet& kw : kwartets)
    {
        if (!kw.IsValid(cards))
        {
            return false;
        }
    }
    return true;
}

const KwartetList& Cards::GetKwartets() const
{
    return kwartets;
}

KwartetList& Cards::GetKwartetsMutable()
{
    return kwartets;
}

const CardList& Cards::GetCards() const
{
    return cards;
}

CardList& Cards::GetCardsMutable()
{
    return cards;
}
