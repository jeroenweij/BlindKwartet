
#include "Kwartetter.h"
#include "MainScreen.h"

Kwartetter::Kwartetter(std::list<std::string> players, const int numCards)
{
    windows.push(std::make_shared<MainScreen>(players));
}

Kwartetter::~Kwartetter()
{
}
