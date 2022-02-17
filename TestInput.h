#pragma once

#include <queue>
#include <string>

#include "InputInterface.h"

struct TestState
{
    int cardCount[4];
    int unknownCardCount[4];
    int kwartetCount[4];
};

class TestInput : public InputInterface
{
  public:
    int         GetInt(const int max) override;
    std::string GetString() override;

    void TestInputPush(std::string string);
    void TestInputPush(int i);
    void PushState(TestState i);

    void PushQuestion(const int to, const int serie, const int card, bool hasCard, const std::string cardName = "", const std::string serieName = "");

  private:
    bool      assert(bool condition, std::string error);
    bool      assertInt(const int i1, const int i2, std::string error);
    bool      CheckState(const PlayerList& players, const Cards& cards);
    const int GetKwartetCount(const Cards& cards, const int plId);

    std::queue<int>         intQ;
    std::queue<std::string> stringQ;
    std::queue<TestState>   stateQ;
};
