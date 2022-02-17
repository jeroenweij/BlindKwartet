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

const int MainScreen::GetKwartetCount(const int plId)
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

void MainScreen::Draw(Window& window)
{
    int       numPlayers = players.size();
    int       space      = windowWidth / numPlayers;
    int       x          = 0;
    const int lines      = 2;
    const int barheigth  = textheigth * lines + margin * (lines + 1);

    // Draw top bar
    window.DrawLine(0, windowHeigth - barheigth, windowWidth, windowHeigth - barheigth, c.black);
    for (const Player& player : players)
    {
        int texty = windowHeigth - textheigth - margin;
        window.DrawLine(x, windowHeigth, x, windowHeigth - barheigth, c.black);
        window.PrintText(x + margin, texty, c.black, player.Name());

        texty -= textheigth + margin;
        int kwartets    = GetKwartetCount(player.GetId());
        int cardsInHand = player.CardsInHand() - (kwartets * 4);

        std::stringstream ss;
        ss << "U:" << player.CardsUnclaimed() << " C:" << cardsInHand << " K:" << kwartets;
        window.PrintText(x + margin, texty, c.black, ss.str());
        x += space;
    }

    // draw kwartets
    x                  = 0;
    int       y        = windowHeigth - barheigth - margin;
    int       kwWidth  = 400;
    const int kwHeigth = (players.size() + 1) * (textheigth + margin * 2);
    const int kwPerRow = windowWidth / (kwWidth + 1);
    int       kwWidtha = windowWidth / kwPerRow - 1;

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

const Player& MainScreen::PlayerFromId(const int plId) const
{
    for (const Player& pl : players)
    {
        if (pl.GetId() == plId)
        {
            return pl;
        }
    }
    return players.front();
}

void MainScreen::DrawKwartet(Window& window, int x, int y, const int w, const int h, const Kwartet& kw)
{
    const int cardW = ((w - 1) / 4) - 1;
    const int cardH = h - textheigth * 2 - 1;

    window.DrawBox(x, y, x + w, y - h, ColorFromState(kw.GetStatus()));
    y -= margin + textheigth;

    std::stringstream ss;

    ss << (kw.GetId() + 1) << ": " << kw.GetName();

    if (kw.GetOwner() != -1)
    {
        ss << "  ->  " << PlayerFromId(kw.GetOwner()).Name();
    }
    else if (kw.GetUnknownClaims() > 0)
    {
        ss << " (";
        for (int plId : kw.GetUnknownClaimPlayers())
        {
            ss << PlayerFromId(plId).Name() << ", ";
        }
        ss << ")";
    }

    window.PrintText(x + margin, y, ColorFromState(kw.GetStatus()), ss.str());
    y -= margin;
    x++;
    int claimedUnnamed = kw.GetUnknownClaims();

    int cardId = 1;

    for (const CardPtr card : cards.GetCards())
    {
        if (card->InKwartet(kw.GetId()))
        {
            Pixel color        = ColorFromState(card->GetStatus());
            int   possiblePlid = -1;
            if (!card->IsNamed() && claimedUnnamed > 0)
            {
                color = c.orange;
                claimedUnnamed--;
                possiblePlid = kw.GetUnknownClaimPlayers().at(claimedUnnamed);
            }
            DrawCard(window, x, y, cardW, cardH, color, card, cardId++, possiblePlid);
            x += cardW + 1;
        }
    }
}

void MainScreen::DrawCard(Window& window, const int x, int y, const int w, const int h, const Pixel& color, const CardPtr card, const int cardId, const int possiblePlId)
{
    window.DrawBox(x, y, x + w, y - h, color);
    std::stringstream ss;

    ss << cardId << ": " << card->GetName();
    y -= margin + textheigth;
    window.PrintText(x + margin, y, color, ss.str());

    y -= margin + textheigth;
    if (card->GetStatus() == Status::UNCLAIMED)
    {
        if (card->IsNamed())
        {
            for (int plId : card->GetNotOwnedByPlayers())
            {
                window.PrintText(x + margin, y, c.orange, PlayerFromId(plId).Name());
                y -= margin + textheigth;
            }
        }
        else if (possiblePlId != -1 && possiblePlId < players.size())
        {
            window.PrintText(x + margin, y, color, PlayerFromId(possiblePlId).Name());
            y -= margin + textheigth;
        }
    }
    else
    {
        window.PrintText(x + margin, y, color, players.at(card->GetOwner()).Name());
    }
}
