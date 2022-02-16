#pragma once

#include <string>
#include <vector>

class Player;

enum class Status
{
    UNCLAIMED,
    CLAIMED,
    KWARTET
};

class CardBase
{
  public:
    CardBase(const int id);
    const std::string GetName() const;
    void              SetName(std::string newName);
    const Status      GetStatus() const;
    const int         GetId() const;
    void              Claim(const int claimPlayer);
    void              ClaimKwartet(const Player& claimPlayer);
    const bool        IsNamed() const;

    const int GetOwner() const;
    void      SetOwner(const int newOwner);

  protected:
    const int id;

  private:
    std::string name;
    Status      status;
    bool        nameSet;
    int         playerId;
};
