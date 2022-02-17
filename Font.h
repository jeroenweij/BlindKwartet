#pragma once
#include <map>
#include <stdint.h>
#include <string>

#include "colors.h"
#include "windowSize.h"

static const unsigned int bitNum = 20;

struct Char
{
    Char(const int ascii, const std::string hex);
    uint32_t bits[bitNum];
    int      bitLength;
    int      startBit;

  private:
    void DetermineBitLength();
};

using CharMap = std::map<int, Char>;
class Font
{
  public:
    Font();

    const bool Load();

    void PrintText(PixelList pixels, uint16_t x, const uint16_t y, const Pixel& color, const std::string text);

  private:
    CharMap charMap;
};
