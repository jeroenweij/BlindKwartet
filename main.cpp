#include <iostream>
#include <list>

#include "Kwartetter.h"

using namespace std;

int main()
{
    int numPlayers = 0;
    int numCards   = 0;
    cout << "Type number of players: ";
    cin >> numPlayers;
    cout << "the number of players is: " << numPlayers << std::endl;

    cout << "Type number of cards per person: ";
    cin >> numCards;
    numCards *= numPlayers;
    cout << "the number of cards is: " << numCards << std::endl;

    if (numPlayers > 1)
    {
        if (numCards % 4 == 0)
        {
            std::list<std::string> players;
            for (int i = 0; i < numPlayers; i++)
            {
                std::string name = "";
                cout << "Type name of player " << (i + 1) << ": ";
                cin >> name;
                cout << "Entered name: " << name << std::endl;
                players.push_back(name);
            }

            Kwartetter(players, numCards);
        }
        else
        {
            cout << "Illegal number of cards: " << numCards << std::endl;
        }
    }
    return 0;
}
