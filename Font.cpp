#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

#include "Font.h"

Font::Font()
{
    Load();
}

const bool Font::Load()
{
    std::string   line;
    std::ifstream hexFile("font.hex");

    std::cout << "Load font" << std::endl;
    if (hexFile.is_open())
    {
        while (std::getline(hexFile, line))
        {
            std::istringstream f(line);
            std::string        asciString;
            std::string        hex;
            getline(f, asciString, ':');
            getline(f, hex, ':');

            int asci = std::stoul(asciString, nullptr, 16);
            if (asci >= ' ' && asci < '~')
            {
                charMap.insert(std::pair<int, Char>(asci, {asci, hex}));
            }
        }
        hexFile.close();
    }
    else
    {
        return false;
    }

    const auto mappedChar = charMap.find(' ');

    if (mappedChar != charMap.end())
    {
        Char ch = mappedChar->second;
        std::cout << " ch.bitLength " << ch.bitLength << std::endl;
        for (int i = 0; i < 21; i++)
        {
            std::cout << std::hex << ch.bits[i] << " ";
        }
        std::cout << std::endl;
    }
    return true;
}

void Font::PrintText(Pixel pixels[heigth][width], uint16_t x, const uint16_t y, const Pixel& color, const std::string text)
{
    for (const char c : text)
    {
        if (c >= ' ' && c <= '~')
        {
            const auto mappedChar = charMap.find(c);
            if (mappedChar != charMap.end())
            {
                Char     ch = mappedChar->second;
                uint32_t y2 = y;
                for (int i = 20; i >= 0; i--)
                {
                    const uint32_t d = ch.bits[i];
                    for (uint32_t x2 = 0; x2 < ch.bitLength; x2++)
                    {
                        if (d & 0x01 << (ch.bitLength - x2))
                        {
                            if (y2 < heigth && x + x2 < width)
                            {
                                pixels[y2][x + x2] = color;
                            }
                        }
                    }
                    y2++;
                }
                x += ch.bitLength;
            }
        }
    }
}

Char::Char(const int asci, const std::string hex)
{
    int numInts    = 21;
    int wordLength = hex.length() / numInts;
    int bitsUsed   = 0;

    for (int i = 0; i < numInts; i++)
    {
        bits[bitsUsed] = std::stoul(hex.substr(i * wordLength, wordLength), nullptr, 16);
        bitsUsed++;
    }

    bitLength = wordLength * 4;
    if (asci == ' ')
    {
        bitLength = 8;
    }
    else
    {
        DetermineBitLength();
    }
}

void Char::DetermineBitLength()
{
    bitLength = 0;
    startBit  = 100;

    for (uint32_t word : bits)
    {
        for (int i = bitLength; i < 32; i++)
        {
            uint32_t mask = 0x01 << i;
            if (word & mask)
            {
                bitLength = std::max(bitLength, i);
                startBit  = std::min(startBit, i);
            }
        }
    }
}
