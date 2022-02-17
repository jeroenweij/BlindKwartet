#include <GL/gl.h>
#include <iostream>

#include "windowSize.h"

#pragma once

struct Pixel
{
    Pixel();
    Pixel(GLubyte r, GLubyte g, GLubyte b);

    GLubyte r;
    GLubyte g;
    GLubyte b;
};

using PixelList = Pixel[windowHeigth][windowWidth];

bool operator==(const Pixel& lhs, const Pixel& rhs);

std::ostream& operator<<(std::ostream& os, const Pixel& p);

class MainColor
{
  public:
    MainColor();
    const Pixel black;
    const Pixel white;
    const Pixel red;
    const Pixel green;
    const Pixel blue;
    const Pixel orange;
    const Pixel yellow;
};
