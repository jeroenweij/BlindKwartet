#include <iostream>
#include <list>

#include "Input.h"
#include "Kwartetter.h"
#include "Player.h"

using namespace std;

int main()
{
    int numPlayers = 0;
    int numCardsPP = 0;
    int numCards   = 0;

    cout << "Type number of players: ";
    cin >> numPlayers;
    cout << "the number of players is: " << numPlayers << std::endl;

    cout << "Type number of cards per person: ";
    cin >> numCardsPP;
    numCards = numPlayers * numCardsPP;
    cout << "the number of cards is: " << numCards << std::endl;

    if (numPlayers > 1)
    {
        if (numCards % 4 == 0)
        {
            PlayerList players;
            for (int i = 0; i < numPlayers; i++)
            {
                std::string name = "";
                cout << "Type name of player " << (i + 1) << ": ";
                cin >> name;
                cout << "Entered name: " << name << std::endl;
                players.push_back({name, numCardsPP});
            }

            Input      input;
            Kwartetter kw(input, players, numCards);
            if (!kw.CheckInit(numCards))
            {
                cout << "Init FAILED" << std::endl;
            }

            kw.Start();
        }
        else
        {
            cout << "Illegal number of cards: " << numCards << std::endl;
        }
    }
    return 0;
}
