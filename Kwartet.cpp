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

    if (unclaimedCards < playersWithUnnamedCard.size())
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
    if (playersWithUnnamedCard.size() < unclaimedCards)
    {
        playersWithUnnamedCard.push_back(pl.GetId());
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
    return playersWithUnnamedCard.size();
}

const std::vector<int>& Kwartet::GetUnknownClaimPlayers() const
{
    return playersWithUnnamedCard;
}

const bool Kwartet::PlayerHasUnnamedCard(const int plId) const
{
    return std::find(playersWithUnnamedCard.begin(), playersWithUnnamedCard.end(), plId) != playersWithUnnamedCard.end();
}

void Kwartet::PlayerLostUnnamedCard(const int plId)
{
    playersWithUnnamedCard.erase(std::find(playersWithUnnamedCard.begin(), playersWithUnnamedCard.end(), plId));
}

void Kwartet::Claim(Player& pl, const CardList& cardlist)
{
    CardBase::ClaimKwartet(pl);
    for (const CardPtr card : cardlist)
    {
        if (card->InKwartet(GetId()))
        {
            if (card->GetStatus() == Status::UNCLAIMED)
            {
                if (PlayerHasUnnamedCard(pl.GetId())) // && !card->IsNamed())
                {
                    PlayerLostUnnamedCard(pl.GetId());
                }
                else
                {
                    pl.ClaimedCards(1);
                }
            }
            card->ClaimKwartet(pl);
        }
    }
    playersWithUnnamedCard.clear();
}
