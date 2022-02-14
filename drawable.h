#pragma once

class Window;
class Drawable
{
  public:
    virtual void Draw(Window& window) = 0;
};
