#include "Player.h"

static int idcounter = 0;

Player::Player(std::string name, const int numCards) :
    id(idcounter++),
    name(name),
    unclaimedCards(numCards),
    claimedCards(0)
{
}

const std::string Player::Name() const
{
    return name;
}

const int Player::GetId() const
{
    return id;
}

const int Player::CardsInHand() const
{
    return unclaimedCards + claimedCards;
}

const int Player::CardsUnclaimed() const
{
    return unclaimedCards;
}

void Player::ClaimedCards(const int numCards)
{
    unclaimedCards -= numCards;
    claimedCards += numCards;
}

void Player::LostCard()
{
    claimedCards--;
}

void Player::AddCard()
{
    claimedCards++;
}

void Player::ClaimCard(CardPtr card)
{
    ClaimedCards(1);
    card->Claim(GetId());
}

const int Player::GetKwartetCount(const KwartetList& kwartets) const
{
    int count = 0;

    for (const Kwartet& kw : kwartets)
    {
        if (kw.GetOwner() == GetId())
        {
            count++;
        }
    }
    return count;
}
