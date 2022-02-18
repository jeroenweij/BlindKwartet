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

enum class QType
{
    INT,
    STRING,
    STATE
};

struct QItem
{
  public:
    QItem(int i);
    QItem(std::string string);
    QItem(TestState& state);

    int         integer;
    std::string string;
    TestState   state;

    QType type;
};

class TestInput : public InputInterface
{
  public:
    int         GetInt(const int max) override;
    std::string GetString() override;
    void        Finish() override;
    bool        CheckState(const PlayerList& players, const Cards& cards) override;

    void Push(std::string string);
    void Push(int i);
    void Push(TestState i);

    void PushQuestion(const int to, const int serie, const int card, bool hasCard, const std::string cardName = "", const std::string serieName = "");

  private:
    bool assert(bool condition, std::string error);
    bool assertInt(const int i1, const int i2, std::string error);

    std::queue<QItem> itemQ;
};
