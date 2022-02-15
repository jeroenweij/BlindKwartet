#include <iostream>
#include <sstream>

#include "Kwartetter.h"
#include "MainScreen.h"

Kwartetter::Kwartetter(PlayerList& players, const int numCards) :
    players(players),
    cards(numCards, players),
    stop(false)
{
    windows.push(std::make_shared<MainScreen>(players, cards));
}

Kwartetter::~Kwartetter()
{
}

bool Kwartetter::CheckInit(const int numCards) const
{
    return cards.CheckInit(numCards);
}

void Kwartetter::Start()
{
    while (HasAnywoneKwartet())
    {
    }
    while (GameIsRunning())
    {
        Player& pl = GetNextPlayer();
        // check if quartet
        Player&  pl2  = AskUser("Aan wie word een kaart gevraagd");
        Kwartet& kw   = AskKwartet();
        CardPtr  card = AskKwartetCard(kw.GetId());
        if (!CheckPlayerHasCardInSeries(pl, kw))
        {
            break;
        }
        if (!CheckPlayer2AnswerYes(pl, pl2, card))
        {
            break;
        }
    }
}

bool Kwartetter::CheckPlayer2AnswerYes(Player& pl1, Player& pl2, const CardPtr card)
{
    std::stringstream ss;
    ss << "Had speler " << pl2.Name() << " de kaart: " << card->GetName();
    if (Ask(ss.str()))
    {
        if (card->GetOwner() != pl2.GetId())
        {
            if (pl2.CardsUnclaimed() > 0)
            {
                pl2.ClaimedCards(1);
            }
            else
            {
                GameOver("Speler kan de gevraagde kaat niet hebben, geen vrije kaarten om toe te wijzen");
                return false;
            }
        }
        pl2.LostCard();
        pl1.AddCard();
        card->Claim(pl1);
        return true;
    }
    else
    {
    }

    GameOver("Onmogelijk antwoord");
    return false;
}

bool Kwartetter::CheckPlayerHasCardInSeries(Player& pl, Kwartet& kw)
{
    for (const CardPtr card : cards.GetCards())
    {
        if (card->InKwartet(kw.GetId()) && card->GetOwner() == pl.GetId())
        {
            return true;
        }
    }

    if (pl.CardsUnclaimed() > 0)
    {
        // Find unnamed card
        for (CardPtr card : cards.GetCardsMutable())
        {
            if (card->InKwartet(kw.GetId()) && card->GetStatus() == Status::UNCLAIMED && !card->IsNamed())
            {
                pl.ClaimCard(card);
                return true;
            }
        }

        // find named card if unnamed not avialable
        for (CardPtr card : cards.GetCardsMutable())
        {
            if (card->InKwartet(kw.GetId()) && card->GetStatus() == Status::UNCLAIMED)
            {
                pl.ClaimCard(card);
                return true;
            }
        }
    }
    else
    {
        GameOver("geen vrije kaarten over om te claimen");
    }

    GameOver("Vragende speler heeft geen kaart in deze serie");
    return false;
}

Player& Kwartetter::GetNextPlayer()
{
    static bool    first = true;
    static Player* pl    = &AskUser("Wie is er aan de beurt?");

    bool ok = false;

    while (!ok)
    {
        if (!first)
        {
            bool get = false;
            for (Player& p : players)
            {
                if (p.GetId() == pl->GetId())
                {
                    get = true;
                }
                else if (get)
                {
                    pl  = &p;
                    get = false;
                    break;
                }
            }
            if (get)
            {
                pl = &players.front();
            }
        }

        first = false;
        std::stringstream s;
        s << "Is player " << pl->Name() << " aan de beurt?";
        ok = Ask(s.str());
    }

    return *pl;
}

bool Kwartetter::Ask(std::string question)
{
    std::string ans;

    std::cout << question << " [y/n]";
    std::cin >> ans;
    char c = std::tolower(ans.c_str()[0]);

    return (c == 'y' || c == 'j');
}

std::string Kwartetter::AskString(std::string question)
{
    std::string ans;

    std::cout << question;
    std::cin >> ans;

    return ans;
}

Player& Kwartetter::AskUser(std::string question)
{
    int ans;

    std::cout << question << ":" << std::endl;
    for (const Player& pl : players)
    {
        std::cout << pl.GetId() << ": " << pl.Name() << std::endl;
    }
    std::cin >> ans;

    if (ans < players.size())
    {
        return players.at(ans);
    }
    return players.back();
}

bool Kwartetter::HasAnywoneKwartet()
{
    if (Ask("Heeft iemand kwartet?"))
    {
        Player& player = AskUser("Wie heeft een kwartet?");
        std::cout << "Speler: " << player.Name() << " heeft een kwartet" << std::endl;
        if (UnclaimedKwartetAvailable())
        {
            Kwartet& kw = UnclaimedKwartetGet();
            kw.ClaimKwartet(player);
            for (CardPtr card : cards.GetCardsMutable())
            {
                if (card->InKwartet(kw.GetId()))
                {
                    card->ClaimKwartet(player);
                }
            }
            if (player.CardsUnclaimed() >= 4)
            {
                player.ClaimedCards(4);
            }
            else
            {
                GameOver("Speler heeft niet genoeg kaarten!");
            }
        }
        else
        {
            GameOver("Er is geen vrij kwartet beschikbaar!");
            return false;
        }
        return true;
    }
    return false;
}

Kwartet& Kwartetter::AskKwartet()
{
    int ans;

    std::cout << "Uit welke serie word er gevraagd:" << std::endl;
    std::cout << "-1: Nieuwe serie" << std::endl;
    for (const Kwartet& kw : cards.GetKwartets())
    {
        if (kw.IsNamed())
        {
            std::cout << kw.GetId() << ": " << kw.GetName() << std::endl;
        }
    }
    std::cin >> ans;

    if (ans == -1)
    {
        // find and use new kwartet
        Kwartet* kwptr = UnNamedKwartetGet();
        if (kwptr == nullptr)
        {
            GameOver("Geen vrij kwartet beschikbaar");
            return cards.GetKwartetsMutable().back();
        }
        kwptr->SetName(AskString("Hoe heet deze serie: "));

        return *kwptr;
    }

    if (ans < cards.GetKwartets().size())
    {
        Kwartet& kw = cards.GetKwartetsMutable().at(ans);
        if (!kw.IsNamed())
        {
            kw.SetName(AskString("Hoe heet deze serie: "));
        }
        return kw;
    }
    return cards.GetKwartetsMutable().back();
}

CardPtr Kwartetter::AskKwartetCard(const int kwId)
{
    int ans;

    std::cout << "Welke kaart word er gevraagd:" << std::endl;
    std::cout << "-1: Nieuwe kaart" << std::endl;

    for (const CardPtr card : cards.GetCards())
    {
        if (card->IsNamed() && card->InKwartet(kwId))
        {
            std::cout << card->GetId() << ": " << card->GetName() << std::endl;
        }
    }
    std::cin >> ans;

    if (ans == -1)
    {
        // find and use new card
        CardPtr card = UnNamedCardGet(kwId);
        if (card == nullptr)
        {
            GameOver("Geen vrije kaart beschikbaar");
            return nullptr;
        }
        card->SetName(AskString("Hoe heet deze kaart: "));

        return card;
    }

    if (ans < cards.GetCards().size())
    {
        CardPtr card = cards.GetCards().at(ans);
        if (!card->IsNamed())
        {
            card->SetName(AskString("Hoe heet deze kaart: "));
        }
        return card;
    }
    return nullptr;
}

CardPtr Kwartetter::UnNamedCardGet(const int kwId)
{
    for (CardPtr card : cards.GetCardsMutable())
    {
        if (card->InKwartet(kwId))
        {
            if (!card->IsNamed() && card->GetStatus() == Status::UNCLAIMED)
            {
                return card;
            }
        }
    }

    for (CardPtr card : cards.GetCardsMutable())
    {
        if (card->InKwartet(kwId))
        {
            if (!card->IsNamed())
            {
                return card;
            }
        }
    }
    return nullptr;
}

Kwartet* Kwartetter::UnNamedKwartetGet()
{
    for (Kwartet& kw : cards.GetKwartetsMutable())
    {
        if (!kw.IsNamed() && kw.GetStatus() == Status::UNCLAIMED)
        {
            return &kw;
        }
    }
    return nullptr;
}

Kwartet& Kwartetter::UnclaimedKwartetGet()
{
    for (Kwartet& kw : cards.GetKwartetsMutable())
    {
        if (kw.GetStatus() == Status::UNCLAIMED)
        {
            return kw;
        }
    }
    return cards.GetKwartetsMutable().back();
}

bool Kwartetter::GameComplete()
{
    for (const Kwartet& kw : cards.GetKwartets())
    {
        if (kw.GetStatus() != Status::KWARTET)
        {
            return false;
        }
    }
    return true;
}

bool Kwartetter::GameIsRunning()
{
    if (GameComplete())
    {
        std::cout << "GAME COMPLETE" << std::endl;
        return false;
    }

    int count = 0;
    for (const Player& p : players)
    {
        count += p.CardsInHand();
    }

    if (count != cards.GetCards().size())
    {
        GameOver("Invalid card count");
        return false;
    }

    return !stop;
}

void Kwartetter::GameOver(std::string reason)
{
    std::cout << "GAME OVER" << std::endl;
    std::cout << reason << std::endl;
    stop = true;
}

const bool Kwartetter::UnclaimedKwartetAvailable() const
{
    for (const Kwartet& kw : cards.GetKwartets())
    {
        if (kw.GetStatus() == Status::UNCLAIMED)
        {
            return true;
        }
    }
    return false;
}
