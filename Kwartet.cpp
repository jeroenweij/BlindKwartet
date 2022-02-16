#include <algorithm>
#include <iostream>
#include <string>

#include "Kwartet.h"
#include "Player.h"

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
    if (cards != 4)
    {
        return false;
    }

    int unclaimedCards = CountUnclaimed(cardlist);

    if (unclaimedCards < playersWithCard.size())
    {
        return false;
    }

    return true;
}

const int Kwartet::CountUnclaimed(const CardList& cardlist) const
{
    int unclaimedCards = 0;

    for (CardPtr card : cardlist)
    {
        if (card->InKwartet(id))
        {
            if (card->GetStatus() == Status::UNCLAIMED)
            {
                unclaimedCards++;
            }
        }
    }
    return unclaimedCards;
}

bool Kwartet::ClaimUnknownCard(const Player& pl, const CardList& cardlist)
{
    int unclaimedCards = CountUnclaimed(cardlist);

    if (unclaimedCards == 0)
    {
        return false;
    }

    if (unclaimedCards == 1)
    {
        return ClaimFirstUnclaimedCard(pl.GetId(), cardlist);
    }
    if (playersWithCard.size() < unclaimedCards)
    {
        playersWithCard.push_back(pl.GetId());
        return true;
    }

    return false;
}

bool Kwartet::ClaimFirstUnclaimedCard(const int plId, const CardList& cardlist)
{
    for (CardPtr card : cardlist)
    {
        if (card->InKwartet(GetId()))
        {
            if (card->GetStatus() == Status::UNCLAIMED)
            {
                card->Claim(plId);
                return true;
            }
        }
    }
    return false;
}

const int Kwartet::GetUnknownClaims() const
{
    return playersWithCard.size();
}

const bool Kwartet::PlayerHasUnnamedCard(const int plId) const
{
    return std::find(playersWithCard.begin(), playersWithCard.end(), plId) != playersWithCard.end();
}

void Kwartet::PlayerLostUnnamedCard(const int plId)
{
    playersWithCard.erase(std::find(playersWithCard.begin(), playersWithCard.end(), plId));
}

void Kwartet::Claim(const Player& pl, const CardList& cardlist)
{
    CardBase::ClaimKwartet(pl);
    playersWithCard.clear();
    for (const CardPtr card : cardlist)
    {
        if (card->InKwartet(GetId()))
        {
            card->ClaimKwartet(pl);
        }
    }
}
