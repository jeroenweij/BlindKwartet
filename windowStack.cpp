
#include "windowStack.h"

WindowStack::WindowStack() :
    stack()
{
    window.Open();
}

WindowStack::~WindowStack()
{
    window.Close();
}

void WindowStack::push(std::shared_ptr<Drawable> screen)
{
    stack.push_back(screen);
    window.SetScreen(screen);
}

void WindowStack::pop()
{
    stack.pop_back();
    window.SetScreen(stack.back());
}
