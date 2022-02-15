#pragma once

#include <memory>
#include <string>
#include <vector>

#include "CardBase.h"

class Player;

class Card;
using CardPtr  = std::shared_ptr<Card>;
using CardList = std::vector<CardPtr>;

class Kwartet;
using KwartetList = std::vector<Kwartet>;

class Card : public CardBase
{
  public:
    Card(const int id, const int kwartetId);
    const bool InKwartet(const int kwId) const;

  private:
    const int kwartetId;
};

class Kwartet : public CardBase
{
  public:
    Kwartet(const int id);

    bool IsValid(const CardList& cardlist) const;

  private:
};
