#include "colors.h"

MainColor::MainColor() :
    black(0, 0, 0),
    white(255, 255, 255),
    red(191, 4, 4),
    green(3, 163, 30),
    blue(13, 4, 194),
    orange(252, 140, 3),
    yellow(255, 255, 0)
{
}

bool operator==(const Pixel& lhs, const Pixel& rhs)
{
    return lhs.r == rhs.r && lhs.g == rhs.g && lhs.b == rhs.b;
}

std::ostream& operator<<(std::ostream& os, const Pixel& p)
{
    os << "RGB(" << (int)p.r << ", " << (int)p.g << ", " << (int)p.b << ")";
    return os;
}
