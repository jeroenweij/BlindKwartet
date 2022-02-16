#pragma once

#include <memory>
#include <string>
#include <vector>

#include "Card.h"
#include "CardBase.h"

class Kwartet;
using KwartetList = std::vector<Kwartet>;

class Kwartet : public CardBase
{
  public:
    Kwartet(const int id);

    bool IsValid(const CardList& cardlist) const;

    bool       ClaimUnknownCard(const Player& pl, const CardList& cardlist);
    const int  GetUnknownClaims() const;
    const bool PlayerHasUnnamedCard(const int plId) const;
    void       PlayerLostUnnamedCard(const int plId);
    void       Claim(const Player& pl, const CardList& cardlist);

  private:
    bool      ClaimFirstUnclaimedCard(const int plId, const CardList& cardlist);
    const int CountUnclaimed(const CardList& cardlist) const;

    std::vector<int> playersWithCard;
};
