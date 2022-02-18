#include <iostream>
#include <string>

#include "TestInput.h"

int TestInput::GetInt(const int max)
{
    if (!itemQ.empty())
    {
        QItem qItem = itemQ.front();
        itemQ.pop();
        if (qItem.type == QType::INT)
        {
            std::cout << " " << qItem.integer << std::endl;
            return qItem.integer;
        }
        else
        {
            std::cout << std::flush;
            throw std::invalid_argument("itemQ has wrong type !Int");
            return 0;
        }
    }
    std::cout << std::flush;
    throw std::invalid_argument("itemQ is empty");
    return 0;
}

std::string TestInput::GetString()
{
    if (!itemQ.empty())
    {
        QItem qItem = itemQ.front();
        itemQ.pop();
        if (qItem.type == QType::STRING)
        {
            std::cout << " " << qItem.string << std::endl;
            return qItem.string;
        }
        else
        {
            std::cout << std::flush;
            throw std::invalid_argument("itemQ has wrong type !String");
            return 0;
        }
    }
    std::cout << std::flush;
    throw std::invalid_argument("itemQ is empty");
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
    if (!itemQ.empty())
    {
        QItem qItem = itemQ.front();
        itemQ.pop();
        if (qItem.type == QType::STATE)
        {
            TestState& state = qItem.state;

            bool result = true;

            for (int i = 0; i < 4; i++)
            {
                result &= assertInt(players.at(i).CardsInHand(), state.cardCount[i] + state.kwartetCount[i] * 4, "Player incorrect cardcount");
                result &= assertInt(players.at(i).CardsUnclaimed(), state.unknownCardCount[i], "Player incorrect unknown cardcount");
                result &= assertInt(GetKwartetCount(cards, i), state.kwartetCount[i], "Player incorrect kwartet count");
            }

            return result;
        }
        else
        {
            std::cout << "Qitem is not a state" << std::endl;
            while (true)
            {
            }
            return false;
        }
    }
    std::cout << "itemQ is empty" << std::flush;
    while (true)
    {
    }
    return false;
}

void TestInput::TestInputPush(std::string string)
{
    std::cout << "Pushing " << string << std::endl;
    itemQ.push({string});
}

void TestInput::TestInputPush(int i)
{
    itemQ.push({i});
}

void TestInput::PushState(TestState i)
{
    itemQ.push({i});
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

QItem::QItem(int i) :
    integer(i),
    type(QType::INT)
{
}

QItem::QItem(std::string string) :
    string(string),
    type(QType::STRING)
{
}

QItem::QItem(TestState& state) :
    state(state),
    type(QType::STATE)
{
}
