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

const std::list<int>& Card::GetNotOwnedByPlayers() const
{
    return notOwnedBy;
}

bool Card::Check(PlayerList& players, KwartetList& kwartets)
{
    if (GetOwner() == -1)
    {
        if (notOwnedBy.size() == players.size() - 1)
        {
            for (Player& p : players)
            {
                if (std::find(notOwnedBy.begin(), notOwnedBy.end(), p.GetId()) == notOwnedBy.end())
                {
                    for (Kwartet& kw : kwartets)
                    {
                        if (kw.GetId() == kwartetId)
                        {
                            if (kw.PlayerHasUnnamedCard(p.GetId()))
                            {
                                kw.PlayerLostUnnamedCard(p.GetId());
                            }
                            break;
                        }
                    }
                    Claim(p.GetId());
                    p.ClaimedCards(1);
                    notOwnedBy.clear();
                    return true;
                }
            }
        }
        else if (notOwnedBy.size() >= players.size())
        {
            return false;
        }
    }
    else
    {
        notOwnedBy.clear();
    }
    return true;
}
