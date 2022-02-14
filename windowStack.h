#pragma once

#include <list>
#include <memory>

#include "drawable.h"
#include "window.h"

class WindowStack
{
  public:
    WindowStack();
    ~WindowStack();

    void push(std::shared_ptr<Drawable> screen);
    void pop();

  private:
    std::list<std::shared_ptr<Drawable>> stack;
    Window                               window;
};
