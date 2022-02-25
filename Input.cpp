#include <ctime>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>

#include "Input.h"

Input::Input()
{
    std::stringstream ss;
    auto              t  = std::time(nullptr);
    auto              tm = *std::localtime(&t);
    ss << "Replay_" << std::put_time(&tm, "%d-%m-%Y_%H-%M-%S") << ".txt";
    fileOut.open(ss.str());
}

int Input::GetInt(const int max)
{
    std::string ans;
    int         i = -100;

    while (i < -1 || i > max)
    {
        try
        {
            std::cin >> ans;
            i = std::stoi(ans);
        }
        catch (const std::exception& e)
        {
            std::cout << "Exception " << e.what() << std::endl;
        }
    }
    fileOut << "input.Push(" << i << ");" << std::endl;
    return i;
}

std::string Input::GetString()
{
    std::string ans;

    std::cin >> ans;

    fileOut << "input.Push(\"" << ans << "\");" << std::endl;
    return ans;
}

bool Input::CheckState(const PlayerList& players, const Cards& cards)
{
    bool        first = true;
    std::string cardsInHand;
    std::string unknowns;
    std::string kwartets;

    for (const Player& pl : players)
    {
        if (!first)
        {
            cardsInHand += ", ";
            unknowns += ", ";
            kwartets += ", ";
        }

        int kwartetCount = pl.GetKwartetCount(cards.GetKwartets());
        cardsInHand += std::to_string(pl.CardsInHand() - kwartetCount * 4);
        unknowns += std::to_string(pl.CardsUnclaimed());
        kwartets += std::to_string(kwartetCount);
        first = false;
    }

    fileOut << "input.Push({{" << cardsInHand << "},{" << unknowns << "},{" << kwartets << "}});\n"
            << std::endl;
    return true;
}

void Input::Finish()
{
    fileOut.close();
}
