#include "MainScreen.h"
#include "colors.h"
#include "window.h"
#include "windowSize.h"

static MainColor c;
MainScreen::MainScreen(std::list<std::string>& players) :
    players(players)
{
}

void MainScreen::Draw(Window& window)
{
    int numPlayers = players.size();
    int space      = width / numPlayers;
    int x          = 0;

    window.DrawLine(0, heigth - 32, width, heigth - 32, c.black);
    for (const std::string& player : players)
    {
        window.DrawLine(x, heigth, x, heigth - 32, c.black);
        window.PrintText(x + 10, heigth - 32 + 8, players.back());
        x += space;
    }
}
