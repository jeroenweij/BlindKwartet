#include <iostream>
#include <string>

#include "TestInput.h"

int TestInput::GetInt(const int max)
{
    if (!intQ.empty())
    {
        int i = intQ.front();
        intQ.pop();
        std::cout << " " << i << std::endl;
        return i;
    }
    std::cout << std::flush;
    throw std::invalid_argument("intQ is empty");
    return 0;
}

std::string TestInput::GetString()
{
    if (!stringQ.empty())
    {
        std::string i = stringQ.front();
        stringQ.pop();
        std::cout << " " << i << std::endl;
        return i;
    }
    std::cout << std::flush;
    throw std::invalid_argument("stringQ is empty");
    return "n";
}

bool TestInput::assert(bool condition, std::string error)
{
    if (!condition)
    {
        std::cout << error << std::endl;
        return false;
    }
    return true;
}

bool TestInput::assertInt(const int i1, const int i2, std::string error)
{
    if (i1 != i2)
    {
        std::cout << error << " " << i1 << " != " << i2 << std::endl;
        return false;
    }
    return true;
}

bool TestInput::CheckState(const PlayerList& players, const Cards& cards)
{
    if (!stateQ.empty())
    {
        TestState state = stateQ.front();
        stateQ.pop();

        bool result = true;

        for (int i = 0; i < 4; i++)
        {
            result &= assertInt(players.at(i).CardsInHand(), state.cardCount[i] + state.kwartetCount[i] * 4, "Player incorrect cardcount");
            result &= assertInt(players.at(i).CardsUnclaimed(), state.unknownCardCount[i], "Player incorrect unknown cardcount");
            result &= assertInt(GetKwartetCount(cards, i), state.kwartetCount[i], "Player incorrect kwartet count");
        }

        return result;
    }
    std::cout << "stateQ is empty" << std::flush;
    while (true)
    {
    }
    return false;
}

void TestInput::TestInputPush(std::string string)
{
    std::cout << "Pushing " << string << std::endl;
    stringQ.push(string);
}

void TestInput::TestInputPush(int i)
{
    intQ.push(i);
}

void TestInput::PushState(TestState i)
{
    stateQ.push(i);
}

void TestInput::PushQuestion(const int to, const int serie, const int card, bool hasCard, const std::string cardName, const std::string serieName)
{
    TestInputPush(to);
    TestInputPush(serie);
    if (!serieName.empty())
    {
        TestInputPush(serieName);
    }
    TestInputPush(card);
    if (!cardName.empty())
    {
        TestInputPush(cardName);
    }
    TestInputPush(hasCard ? "y" : "n");
}

const int TestInput::GetKwartetCount(const Cards& cards, const int plId)
{
    int count = 0;

    for (const Kwartet& kw : cards.GetKwartets())
    {
        if (kw.GetOwner() == plId)
        {
            count++;
        }
    }
    return count;
}
