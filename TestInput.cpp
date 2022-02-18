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

void TestInput::Finish()
{
    if (!itemQ.empty())
    {
        throw std::invalid_argument("itemQ is NOT empty at end of program");
    }
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
    bool result = false;
    if (!itemQ.empty())
    {
        QItem qItem = itemQ.front();
        itemQ.pop();
        if (qItem.type == QType::STATE)
        {
            TestState& state = qItem.state;

            result = true;

            for (int i = 0; i < 4; i++)
            {
                result &= assertInt(players.at(i).CardsInHand(), state.cardCount[i] + state.kwartetCount[i] * 4, "Player incorrect cardcount");
                result &= assertInt(players.at(i).CardsUnclaimed(), state.unknownCardCount[i], "Player incorrect unknown cardcount");
                result &= assertInt(players.at(i).GetKwartetCount(cards.GetKwartets()), state.kwartetCount[i], "Player incorrect kwartet count");
            }
        }
        else
        {
            std::cout << "Qitem is not a state" << std::endl;
            result = false;
        }
    }
    else
    {
        std::cout << "itemQ is empty" << std::endl;
    }

    std::cout << "enter to continue" << std::endl;
    std::cin.ignore();
    return result;
}

void TestInput::Push(std::string string)
{
    itemQ.push({string});
}

void TestInput::Push(int i)
{
    itemQ.push({i});
}

void TestInput::Push(TestState i)
{
    itemQ.push({i});
}

void TestInput::PushQuestion(const int to, const int serie, const int card, bool hasCard, const std::string cardName, const std::string serieName)
{
    Push(to);
    Push(serie);
    if (!serieName.empty())
    {
        Push(serieName);
    }
    Push(card);
    if (!cardName.empty())
    {
        Push(cardName);
    }
    Push(hasCard ? "y" : "n");
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
