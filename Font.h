#pragma once
#include <map>
#include <stdint.h>
#include <string>

#include "colors.h"
#include "windowSize.h"

static const unsigned int bitNum = 21;
struct Char
{
    Char(const int ascii, const std::string hex);
    uint32_t bits[bitNum];
    int      bitLength;
    int      startBit;

  private:
    void DetermineBitLength();
};

class Font
{
  public:
    Font();

    const bool Load();

    void PrintText(Pixel pixels[heigth][width], uint16_t x, const uint16_t y, const Pixel& color, const std::string text);

  private:
    std::map<int, Char> charMap;
};
