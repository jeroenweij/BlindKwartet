#include <string>

#include "CardBase.h"
#include "Player.h"

const int CardBase::GetId() const
{
    return id;
}

void CardBase::Claim(const Player& claimPlayer)
{
    playerId = claimPlayer.GetId();
    status   = Status::CLAIMED;
}

void CardBase::ClaimKwartet(const Player& claimPlayer)
{
    playerId = claimPlayer.GetId();
    status   = Status::KWARTET;
}

const bool CardBase::IsNamed() const
{
    return nameSet;
}

const int CardBase::GetOwner() const
{
    return playerId;
}

CardBase::CardBase(const int id) :
    id(id),
    name(""),
    status(Status::UNCLAIMED),
    nameSet(false),
    playerId(-1)
{
}

const std::string CardBase::GetName() const
{
    static std::string unnamed = "XXX";
    if (nameSet)
    {
        return name;
    }
    return unnamed;
}

void CardBase::SetName(std::string newName)
{
    name    = newName;
    nameSet = true;
}

const Status CardBase::GetStatus() const
{
    return status;
}
