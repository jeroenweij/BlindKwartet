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

static void scenario1(TestInput& input)
{
    input.Push("n");
    input.Push({{4, 4, 4, 4}, {4, 4, 4, 4}, {0, 0, 0, 0}});

    // p1 begint
    input.Push(1);

    // p1 -> p2
    input.PushQuestion(2, B, 2, true, "2", "B");
    input.Push("n");

    input.Push({{5, 3, 4, 4}, {3, 3, 4, 4}, {0, 0, 0, 0}});
    input.Push("y");

    // p1 -> p3
    input.PushQuestion(3, B, 1, false, "1");
    input.Push({{5, 3, 4, 4}, {3, 3, 4, 4}, {0, 0, 0, 0}});

    // p2 -> p4
    input.Push("n");
    input.Push(2);
    input.PushQuestion(4, D, 2, false, "2", "D");
    input.Push({{5, 3, 4, 4}, {3, 2, 4, 4}, {0, 0, 0, 0}});

    // p3 -> p4
    input.Push("n");
    input.Push(3);
    input.PushQuestion(4, A, 4, true, "4", "A");
    input.Push("n");
    input.Push({{5, 3, 5, 3}, {3, 2, 3, 3}, {0, 0, 0, 0}});

    // p3 -> p2
    input.Push("y");
    input.PushQuestion(2, A, 3, false, "3");
    input.Push({{5, 3, 5, 3}, {3, 2, 3, 3}, {0, 0, 0, 0}});

    // p4 -> p2
    input.Push("n");
    input.Push(4);
    input.PushQuestion(2, C, 2, true, "2", "C");
    input.Push("n");
    input.Push({{5, 2, 5, 4}, {3, 1, 3, 2}, {0, 0, 0, 0}});

    // p4 -> p3
    input.Push("y");
    input.PushQuestion(3, C, 3, true, "3");
    input.Push("n");
    input.Push({{5, 2, 4, 5}, {3, 1, 2, 2}, {0, 0, 0, 0}});

    // p4 -> p2
    input.Push("y");
    input.PushQuestion(2, B, 2, false);
    input.Push({{5, 2, 4, 5}, {3, 1, 2, 1}, {0, 0, 0, 0}});

    // p1 -> p4
    input.Push("n");
    input.Push(1);
    input.PushQuestion(4, B, 1, true);
    input.Push("n");
    input.Push({{6, 2, 4, 4}, {3, 1, 2, 1}, {0, 0, 0, 0}});

    // p1 -> p3
    input.Push("y");
    input.PushQuestion(3, B, 4, true, "4");
    input.Push({{3, 2, 3, 4}, {3, 1, 1, 1}, {1, 0, 0, 0}});

    // p1 -> p3
    input.Push("y");
    input.PushQuestion(3, D, 3, true, "3");
    input.Push({{4, 2, 2, 4}, {2, 1, 0, 1}, {1, 0, 0, 0}});

    // p1 -> p2
    input.Push("y");
    input.PushQuestion(2, D, 4, true, "4");
    input.Push("y");
    input.Push({{1, 1, 2, 4}, {1, 1, 0, 1}, {2, 0, 0, 0}});

    // p1 -> p3
    input.Push("y");
    input.PushQuestion(3, C, 2, false);
    input.Push({{1, 1, 2, 4}, {0, 1, 0, 1}, {2, 0, 0, 0}});

    // p2 -> p3

    input.Push("n");
    input.Push(2);
    input.PushQuestion(3, A, 1, true, "1");
    input.Push({{1, 2, 1, 4}, {0, 0, 0, 1}, {2, 0, 0, 0}});

    // p2 -> p3
    input.Push("y");
    input.PushQuestion(3, A, 4, true);
    input.Push({{1, 3, 0, 4}, {0, 0, 0, 1}, {2, 0, 0, 0}});

    // p2 -> p4
    input.Push("y");
    input.PushQuestion(4, A, 3, true);
    input.Push({{1, 0, 0, 3}, {0, 0, 0, 0}, {2, 1, 0, 0}});

    // p4 -> p1
    input.Push("n");
    input.Push(4);
    input.PushQuestion(1, C, 4, true, "4");
    input.Push({{0, 0, 0, 0}, {0, 0, 0, 0}, {2, 1, 0, 1}});
}

int main()
{
    int numPlayers = 4;
    int numCardsPP = 4;

    PlayerList players;
    players.push_back({"PL1", numCardsPP});
    players.push_back({"PL2", numCardsPP});
    players.push_back({"PL3", numCardsPP});
    players.push_back({"PL4", numCardsPP});

    TestInput input;

    scenario1(input);

    Kwartetter kw(input, players, numPlayers * numCardsPP);
    kw.Start();

    return 0;
}
