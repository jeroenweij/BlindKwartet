#include <iostream>
#include <list>

#include "Kwartetter.h"
#include "Player.h"
#include "TestInput.h"

using namespace std;

enum LetterToNumber
{
    A = 1,
    B,
    C,
    D
};

int main()
{
    int numPlayers = 4;
    int numCardsPP = 4;
    int numCards   = 0;

    numCards = numPlayers * numCardsPP;
    cout << "the number of cards is: " << numCards << std::endl;

    PlayerList players;
    players.push_back({"PL1", numCardsPP});
    players.push_back({"PL2", numCardsPP});
    players.push_back({"PL3", numCardsPP});
    players.push_back({"PL4", numCardsPP});

    TestInput input;
    input.TestInputPush("n");
    input.PushState({{4, 4, 4, 4}, {4, 4, 4, 4}, {0, 0, 0, 0}});

    // p1 begint
    input.TestInputPush(1);
    input.TestInputPush("y");

    // p1 -> p2
    input.PushQuestion(2, B, 2, true, "2", "B");
    input.TestInputPush("n");

    input.PushState({{5, 3, 4, 4}, {3, 3, 4, 4}, {0, 0, 0, 0}});
    input.TestInputPush("y");

    // p1 -> p3
    input.PushQuestion(3, B, 1, false, "1");
    input.PushState({{5, 3, 4, 4}, {3, 3, 4, 4}, {0, 0, 0, 0}});

    // p2 -> p4
    input.TestInputPush("y");
    input.PushQuestion(4, D, 2, false, "2", "D");
    input.PushState({{5, 3, 4, 4}, {3, 2, 4, 4}, {0, 0, 0, 0}});

    // p3 -> p4
    input.TestInputPush("y");
    input.PushQuestion(4, A, 4, true, "4", "A");
    input.TestInputPush("n");
    input.PushState({{5, 3, 5, 3}, {3, 2, 3, 3}, {0, 0, 0, 0}});

    // p3 -> p2
    input.TestInputPush("y");
    input.PushQuestion(2, A, 3, false, "3");
    input.PushState({{5, 3, 5, 3}, {3, 2, 3, 3}, {0, 0, 0, 0}});

    // p4 -> p2
    input.TestInputPush("y");
    input.PushQuestion(2, C, 2, true, "2", "C");
    input.TestInputPush("n");
    input.PushState({{5, 2, 5, 4}, {3, 1, 3, 2}, {0, 0, 0, 0}});

    // p4 -> p3
    input.TestInputPush("y");
    input.PushQuestion(3, C, 3, true, "3");
    input.TestInputPush("n");
    input.PushState({{5, 2, 4, 5}, {3, 1, 2, 2}, {0, 0, 0, 0}});

    // p4 -> p2
    input.TestInputPush("y");
    input.PushQuestion(2, B, 2, false);
    input.PushState({{5, 2, 4, 5}, {3, 1, 2, 1}, {0, 0, 0, 0}});

    // p1 -> p4
    input.TestInputPush("y");
    input.PushQuestion(4, B, 1, true);
    input.TestInputPush("n");
    input.PushState({{6, 2, 4, 4}, {3, 1, 2, 1}, {0, 0, 0, 0}});

    // p1 -> p3
    input.TestInputPush("y");
    input.PushQuestion(3, B, 4, true, "4");
    input.PushState({{3, 2, 3, 4}, {3, 1, 1, 1}, {1, 0, 0, 0}});

    // p1 -> p3
    input.TestInputPush("y");
    input.PushQuestion(3, D, 3, true, "3");
    input.PushState({{4, 2, 2, 4}, {2, 1, 0, 1}, {1, 0, 0, 0}});

    // p1 -> p2
    input.TestInputPush("y");
    input.PushQuestion(2, D, 4, true, "4");
    input.TestInputPush("y");
    input.PushState({{1, 1, 2, 4}, {1, 1, 0, 1}, {2, 0, 0, 0}});

    // p1 -> p3
    input.TestInputPush("y");
    input.PushQuestion(3, C, 2, false);
    input.PushState({{1, 1, 2, 4}, {0, 1, 0, 1}, {2, 0, 0, 0}});

    // p2 -> p3
    input.TestInputPush("y");
    input.PushQuestion(3, A, 1, true, "1");
    input.PushState({{1, 2, 1, 4}, {0, 0, 0, 1}, {2, 0, 0, 0}});

    // p2 -> p3
    input.TestInputPush("y");
    input.PushQuestion(3, A, 4, true);
    input.PushState({{1, 3, 0, 4}, {0, 0, 0, 1}, {2, 0, 0, 0}});

    // p2 -> p4
    input.TestInputPush("y");
    input.PushQuestion(4, A, 3, true);
    input.PushState({{1, 0, 0, 3}, {0, 0, 0, 0}, {2, 1, 0, 0}});

    // player 2 geen kaarten meer
    input.TestInputPush("n");
    // player 3 geen kaarten meer
    input.TestInputPush("n");

    // p4 -> p1
    input.TestInputPush("y");
    input.PushQuestion(1, C, 4, true, "4");
    input.PushState({{0, 0, 0, 0}, {0, 0, 0, 0}, {2, 1, 0, 1}});

    Kwartetter kw(input, players, numCards);
    if (!kw.CheckInit(numCards))
    {
        cout << "Init FAILED" << std::endl;
    }

    kw.Start();

    return 0;
}
