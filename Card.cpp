#include <algorithm>
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

void Card::NotOwnedBy(const int playerId)
{
    if (GetOwner() == -1)
    {
        if (notOwnedBy.empty() || std::find(notOwnedBy.begin(), notOwnedBy.end(), playerId) == notOwnedBy.end())
        {
            notOwnedBy.push_back(playerId);
        }
    }
}

bool Card::Check(const PlayerList& players)
{
    if (GetOwner() == -1)
    {
        if (notOwnedBy.size() == players.size() - 1)
        {
            std::cout << "Card " << GetName() << "Is not owned by players: " << std::endl;
            for (const int i : notOwnedBy)
            {
                std::cout << i << ", ";
            }
            std::cout << std::endl;

            for (const Player& p : players)
            {
                if (std::find(notOwnedBy.begin(), notOwnedBy.end(), p.GetId()) == notOwnedBy.end())
                {
                    std::cout << "Check p " << p.Name() << " NOT FOUND: " << p.GetId() << std::endl;
                    Claim(p.GetId());
                    notOwnedBy.clear();
                    return true;
                }
            }
        }
    }
    else
    {
        notOwnedBy.clear();
    }
    return true;
}
