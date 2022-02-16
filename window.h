#pragma once

#include <GLFW/glfw3.h>
#include <list>
#include <thread>

#include "Font.h"
#include "colors.h"
#include "drawable.h"
#include "windowSize.h"

class Window
{
  public:
    Window();
    void Open();
    void Close();
    void SetScreen(std::shared_ptr<Drawable> newScreen);

    void DrawLine(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, const Pixel& color);
    void DrawBox(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, const Pixel& color);
    void DrawBoxFill(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, const Pixel& color);
    void DrawCircle(const uint16_t x, const uint16_t y, const uint16_t radius, const uint16_t numberOfSides, const Pixel& color, bool fill = false);
    void Fill(const uint16_t x, const uint16_t y, const Pixel& color);

    void PrintText(const uint16_t x, const uint16_t y, const Pixel& color, const std::string text);

  private:
    void Thread();
    void FillRecursive(const uint16_t x, const uint16_t y, const Pixel oldColor, const Pixel& color, const uint16_t c);

    void Clear();

    Pixel                     pixels[heigth][width];
    std::thread               thread;
    GLFWwindow*               window;
    std::shared_ptr<Drawable> screen;
    Font                      textRender;
};
