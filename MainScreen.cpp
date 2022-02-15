#include <sstream>

#include "Card.h"
#include "MainScreen.h"
#include "colors.h"
#include "window.h"
#include "windowSize.h"

static MainColor c;

const int margin     = 8;
const int textheigth = 16;

MainScreen::MainScreen(const PlayerList& players, const Cards& cards) :
    players(players),
    cards(cards)
{
}

void MainScreen::Draw(Window& window)
{
    int       numPlayers = players.size();
    int       space      = width / numPlayers;
    int       x          = 0;
    const int lines      = 2;
    const int barheigth  = textheigth * lines + margin * (lines + 1);

    // Draw top bar
    window.DrawLine(0, heigth - barheigth, width, heigth - barheigth, c.black);
    for (const Player& player : players)
    {
        int texty = heigth - textheigth - margin;
        window.DrawLine(x, heigth, x, heigth - barheigth, c.black);
        window.PrintText(x + margin, texty, c.black, player.Name());

        texty -= textheigth + margin;

        std::stringstream ss;
        ss << "U:" << player.CardsUnclaimed() << " C:" << player.CardsInHand() << " K:" << player.Kwartets();
        window.PrintText(x + margin, texty, c.black, ss.str());
        x += space;
    }

    // draw kwartets
    x                  = 0;
    int       y        = heigth - barheigth - 1;
    int       kwWidth  = 400;
    const int kwHeigth = (players.size() + 1) * (textheigth + margin * 2);
    const int kwPerRow = width / (kwWidth + 1);
    int       kwWidtha = width / kwPerRow - 1;

    int columnCount = 0;
    for (const Kwartet& kw : cards.GetKwartets())
    {
        if (y < kwHeigth)
        {
            break;
        }
        DrawKwartet(window, x, y, kwWidth, kwHeigth, kw);
        x += kwWidtha + 1;
        columnCount++;
        if (columnCount == kwPerRow)
        {
            columnCount = 0;
            y -= kwHeigth + 1 + margin;
            x = 0;
        }
    }
}

const Pixel& MainScreen::ColorFromState(Status state)
{
    switch (state)
    {
        case Status::UNCLAIMED:
            return c.green;
        case Status::CLAIMED:
            return c.red;
        case Status::KWARTET:
            return c.blue;
    }

    return c.black;
}

void MainScreen::DrawKwartet(Window& window, int x, int y, const int w, const int h, const Kwartet& kw)
{
    const int cardW = ((w - 1) / 4) - 1;
    const int cardH = h - textheigth * 2 - 1;

    window.DrawBox(x, y, x + w, y - h, ColorFromState(kw.GetStatus()));
    y -= margin + textheigth;

    std::stringstream ss;
    ss << kw.GetId() << ": " << kw.GetName();
    window.PrintText(x + margin, y, ColorFromState(kw.GetStatus()), ss.str());
    y -= margin;
    x++;
    for (const CardPtr card : cards.GetCards())
    {
        if (card->InKwartet(kw.GetId()))
        {
            DrawCard(window, x, y, cardW, cardH, card);
            x += cardW + 1;
        }
    }
}

void MainScreen::DrawCard(Window& window, const int x, int y, const int w, const int h, const CardPtr card)
{

    window.DrawBox(x, y, x + w, y - h, ColorFromState(card->GetStatus()));
    std::stringstream ss;
    ss << card->GetId() << ": " << card->GetName();
    y -= margin + textheigth;
    window.PrintText(x + margin, y, ColorFromState(card->GetStatus()), ss.str());

    y -= margin + textheigth;
    if (card->GetStatus() != Status::UNCLAIMED)
    {
        window.PrintText(x + margin, y, ColorFromState(card->GetStatus()), players.at(card->GetOwner()).Name());
    }
}
