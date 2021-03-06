#include <iostream>
#include <sstream>

#include "Kwartetter.h"
#include "MainScreen.h"

Kwartetter::Kwartetter(InputInterface& input, PlayerList& players, const int numCards) :
    input(input),
    players(players),
    cards(numCards, players),
    stop(false),
    nextPlayer(-1)
{
    windows.push(std::make_shared<MainScreen>(players, cards));
}

Kwartetter::~Kwartetter()
{
}

bool Kwartetter::CheckInit(const int numCards)
{
    return cards.CheckInit(numCards);
}

void Kwartetter::Start()
{
    while (HasAnywoneKwartet() && !stop)
    {
    }

    while (GameIsRunning())
    {
        bool valid = true;

        Player& pl = GetNextPlayer(valid);
        if (!valid)
        {
            continue;
        }

        std::cout << std::endl;
        std::cout << "Speler " << pl.Name() << " is aan zet" << std::endl;
        Player& pl2 = AskUser("Aan wie word een kaart gevraagd", valid);
        if (!valid)
        {
            continue;
        }

        Kwartet& kw = AskKwartet(valid);
        if (!valid)
        {
            continue;
        }
        CardPtr card = AskKwartetCard(kw.GetId(), valid);
        if (!valid)
        {
            continue;
        }

        if (!CheckPlayerHasCardInSeries(pl, kw))
        {
            break;
        }
        if (!CheckPlayer2Answer(pl, pl2, card, kw))
        {
            break;
        }
    }

    if (!input.CheckState(players, cards))
    {
        std::cout << "Game finished in invalid state" << std::endl;
    }
    input.Finish();
}

bool Kwartetter::CheckPlayer2Answer(Player& pl1, Player& pl2, const CardPtr card, Kwartet& kw)
{
    std::stringstream ss;

    ss << "Had speler " << pl2.Name() << " de kaart: " << kw.GetName() << " : " << card->GetName();
    if (Ask(ss.str()))
    {
        if (card->GetOwner() != pl2.GetId())
        {
            if (kw.PlayerHasUnnamedCard(pl2.GetId()))
            {
                kw.PlayerLostUnnamedCard(pl2.GetId());
            }
            else
            {
                if (pl2.CardsUnclaimed() > 0)
                {
                    pl2.ClaimedCards(1);
                }
                else
                {
                    GameOver("Speler kan de gevraagde kaart niet hebben, geen vrije kaarten om toe te wijzen");
                    return false;
                }
            }
        }
        pl2.LostCard();
        pl1.AddCard();
        card->Claim(pl1.GetId());
        return CheckNewKwartet(pl1, kw);
    }
    else
    {
        nextPlayer = pl2.GetId();
        if (card->GetOwner() == pl2.GetId())
        {
            std::stringstream ss;
            ss << "Speler " << pl2.Name() << " had de kaart " << card->GetName() << "wel!" << std::endl;
            GameOver(ss.str());
            return false;
        }
        else
        {
            card->NotOwnedBy(pl2.GetId());
            card->NotOwnedBy(pl1.GetId());
        }
        return true;
    }

    GameOver("Onmogelijk antwoord");
    return false;
}

bool Kwartetter::CheckNewKwartet(Player& pl1, Kwartet& kw)
{
    int playerclaimed = 0;

    for (const int plId : kw.GetUnknownClaimPlayers())
    {
        if (plId != pl1.GetId())
        {
            std::cout << "player " << pl1.Name() << " Kan onmogelijk het kwartet " << kw.GetName() << " heben" << std::endl;
            return true;
        }
    }

    for (const CardPtr card : cards.GetCards())
    {
        if (card->InKwartet(kw.GetId()))
        {
            if (card->GetOwner() != pl1.GetId() && card->GetStatus() != Status::UNCLAIMED)
            {
                std::cout << "player " << pl1.Name() << " Kan onmogelijk het kwartet " << kw.GetName() << " heben" << std::endl;
                return true;
            }
            if (card->GetOwner() == pl1.GetId())
            {
                playerclaimed++;
            }
        }
    }

    if (playerclaimed == 4 || (playerclaimed == 3 and kw.PlayerHasUnnamedCard(pl1.GetId())))
    {
        std::cout << "player " << pl1.Name() << " Heeft het kwartet " << kw.GetName() << " compleet" << std::endl;
        kw.Claim(pl1, cards.GetCards());
        return true;
    }

    int missing = 4 - playerclaimed - (kw.PlayerHasUnnamedCard(pl1.GetId()) ? 1 : 0);

    if (pl1.CardsUnclaimed() < missing)
    {
        std::cout << "player " << pl1.Name() << " heeft niet genoeg vrije kaarten om het kwartet " << kw.GetName() << " te heben" << std::endl;
        return true;
    }

    std::stringstream ss;

    ss << "Heeft " << pl1.Name() << " het kwartet " << kw.GetName() << " compleet?";
    if (Ask(ss.str()))
    {
        kw.Claim(pl1, cards.GetCards());
    }

    return true;
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

    if (kw.PlayerHasUnnamedCard(pl.GetId()))
    {
        return true;
    }

    if (pl.CardsUnclaimed() > 0)
    {
        if (kw.ClaimUnknownCard(pl, cards.GetCards()))
        {
            pl.ClaimedCards(1);
            return true;
        }
        else
        {
            GameOver("Kan geen kaart claimen in dit kwartet");
        }
    }
    else
    {
        GameOver("geen vrije kaarten over om te claimen");
    }

    GameOver("Vragende speler heeft geen kaart in deze serie");
    return false;
}

Player& Kwartetter::GetNextPlayer(bool& validAns)
{
    if (nextPlayer >= 0 && nextPlayer < players.size())
    {
        std::stringstream s;
        s << "Is speler " << players.at(nextPlayer).Name() << " aan de beurt?";
        if (Ask(s.str()))
        {
            validAns = true;
            return players.at(nextPlayer);
        }
    }

    Player& newUser = AskUser("Wie is er aan de beurt?", validAns);
    nextPlayer      = newUser.GetId();
    return newUser;
}

bool Kwartetter::Ask(std::string question)
{
    std::cout << std::endl;
    std::cout << question << " [y/n]";
    std::string ans = input.GetString();

    char c = std::tolower(ans.c_str()[0]);

    return (c == 'y' || c == 'j');
}

std::string Kwartetter::AskString(std::string question)
{
    std::string ans;

    std::cout << question;
    return input.GetString();
}

Player& Kwartetter::AskUser(std::string question, bool& validAns)
{
    std::cout << std::endl;
    std::cout << question << ":" << std::endl;
    validAns = false;

    for (const Player& pl : players)
    {
        std::cout << pl.GetId() + 1 << ": " << pl.Name() << std::endl;
    }
    int ans = input.GetInt(players.size());

    if (ans <= players.size() && ans > 0)
    {
        validAns = true;
        return players.at(ans - 1);
    }
    return players.back();
}

bool Kwartetter::HasAnywoneKwartet()
{
    if (Ask("Heeft iemand kwartet?"))
    {
        bool    valid  = false;
        Player& player = AskUser("Wie heeft een kwartet?", valid);
        if (!valid)
        {
            return true;
        }

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

Kwartet& Kwartetter::AskKwartet(bool& valid)
{
    std::cout << std::endl;
    std::cout << "Uit welke serie word er gevraagd:" << std::endl;
    std::cout << "0: Nieuwe serie" << std::endl;
    for (const Kwartet& kw : cards.GetKwartets())
    {
        if (kw.IsNamed())
        {
            std::cout << (kw.GetId() + 1) << ": " << kw.GetName() << std::endl;
        }
    }
    valid   = false;
    int ans = input.GetInt(cards.GetKwartets().size());

    if (ans == 0)
    {
        // find and use new kwartet
        Kwartet* kwptr = UnNamedKwartetGet();
        if (kwptr == nullptr)
        {
            GameOver("Geen vrij kwartet beschikbaar");
            return cards.GetKwartetsMutable().back();
        }
        kwptr->SetName(AskString("Hoe heet deze serie: "));

        valid = true;
        return *kwptr;
    }

    if (ans >= 0 && ans <= cards.GetKwartets().size())
    {
        Kwartet& kw = cards.GetKwartetsMutable().at(ans - 1);
        if (!kw.IsNamed())
        {
            kw.SetName(AskString("Hoe heet deze serie: "));
        }
        valid = true;
        return kw;
    }
    return cards.GetKwartetsMutable().back();
}

CardPtr Kwartetter::AskKwartetCard(const int kwId, bool& valid)
{
    std::cout << std::endl;
    std::cout << "Welke kaart word er gevraagd:" << std::endl;
    std::cout << "0: Nieuwe kaart" << std::endl;

    valid      = false;
    int offset = -1;
    int count  = 1;

    for (const CardPtr card : cards.GetCards())
    {
        if (card->InKwartet(kwId))
        {
            if (offset < 0)
            {
                offset = card->GetId();
            }
            if (card->IsNamed())
            {
                std::cout << count << ": " << card->GetName() << std::endl;
            }
            count++;
        }
    }
    int ans = input.GetInt(4);

    if (ans == 0)
    {
        // find and use new card
        CardPtr card = UnNamedCardGet(kwId);
        if (card == nullptr)
        {
            GameOver("Geen vrije kaart beschikbaar");
            return nullptr;
        }
        card->SetName(AskString("Hoe heet deze kaart: "));

        valid = true;
        return card;
    }

    if (ans >= 0 && ans < cards.GetCards().size())
    {
        CardPtr card = cards.GetCards().at(offset + ans - 1);
        if (!card->IsNamed())
        {
            card->SetName(AskString("Hoe heet deze kaart: "));
        }
        valid = true;
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
    int count = 0;

    for (const Player& p : players)
    {
        count += p.CardsInHand();
        if (p.CardsUnclaimed() > p.CardsInHand())
        {
            std::stringstream ss;
            ss << "Speler " << p.Name() << " heeft meer onbekende kaarten dan kaarten";
            GameOver(ss.str());
            return false;
        }

        if (p.CardsInHand() < 0)
        {
            std::stringstream ss;
            ss << "Speler " << p.Name() << " heeft een negatief aantal kaarten";
            GameOver(ss.str());
            return false;
        }

        if (p.CardsUnclaimed() < 0)
        {
            std::stringstream ss;
            ss << "Speler " << p.Name() << " heeft een negatief aantal onbekende kaarten";
            GameOver(ss.str());
            return false;
        }
    }

    if (count != cards.GetCards().size())
    {
        GameOver("Invalid card count");
        return false;
    }

    for (const CardPtr card : cards.GetCards())
    {
        if (!card->Check(players, cards.GetKwartetsMutable()))
        {
            std::stringstream ss;
            ss << "Kaart " << card->GetName() << " is in een invalide staat";
            GameOver(ss.str());
            return false;
        }
    }

    for (Kwartet& kw : cards.GetKwartetsMutable())
    {
        if (!kw.IsValid(cards.GetCards()))
        {
            std::stringstream ss;
            ss << "Kwarted " << kw.GetName() << " is in een ongeldige staat";
            GameOver(ss.str());
            return false;
        }
    }

    if (GameComplete())
    {
        std::cout << "GAME COMPLETE" << std::endl;
        return false;
    }

    if (!input.CheckState(players, cards))
    {
        GameOver("Invallid state");
        return false;
    }

    return !stop;
}

void Kwartetter::GameOver(std::string reason)
{
    std::cout << "?????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????" << std::endl;
    std::cout << "?????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????" << std::endl;
    std::cout << "?????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????" << std::endl;
    std::cout << "?????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????" << std::endl;
    std::cout << "?????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????" << std::endl;
    std::cout << "?????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????" << std::endl;

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
