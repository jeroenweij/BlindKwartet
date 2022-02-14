#include <GLFW/glfw3.h>
#include <iostream>
#include <math.h>
#include <thread>

#include "textRender.h"
#include "window.h"

static MainColor colors;

void Window::Thread()
{
    /* Initialize the library */
    if (!glfwInit())
    {
        exit(1);
    }

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(width, heigth, "Blind kwartetter", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        exit(1);
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT);

        Clear();
        if (screen)
        {
            screen->Draw(*this);
        }
        else
        {
            Draw();
        }

        glDrawPixels(width, heigth, GL_RGB, GL_UNSIGNED_BYTE, pixels);

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    glfwTerminate();
}

Window::Window() :
    thread(),
    window(nullptr)
{
    Clear();
}

void Window::Clear()
{
    for (int w = 0; w < width; w++)
    {
        for (int h = 0; h < heigth; h++)
        {
            pixels[h][w] = {255, 255, 255};
        }
    }
}

void Window::Close()
{
    thread.join();
}

void Window::SetScreen(std::shared_ptr<Drawable> newScreen)
{
    screen = newScreen;
}

void Window::DrawLine(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, const Pixel& color)
{
    if (abs(x2 - x1) > abs(y2 - y1))
    {
        if (x1 > x2)
        {
            std::swap(x1, x2);
            std::swap(y1, y2);
        }
        for (uint16_t x = x1; x < x2; x++)
        {
            const uint16_t y = (y2 - y1) * (x - x1) / (x2 - x1) + y1;
            pixels[y][x]     = color;
        }
    }
    else
    {
        if (y1 > y2)
        {
            std::swap(x1, x2);
            std::swap(y1, y2);
        }
        for (uint16_t y = y1; y < y2; y++)
        {
            const uint16_t x = (x2 - x1) * (y - y1) / (y2 - y1) + x1;
            pixels[y][x]     = color;
        }
    }
}

void Window::DrawBox(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, const Pixel& color)
{
    if (x1 > x2)
    {
        std::swap(x1, x2);
    }
    if (y1 > y2)
    {
        std::swap(y1, y2);
    }
    for (uint16_t x = x1; x < x2; x++)
    {
        for (uint16_t y = y1; y < y2; y++)
        {
            pixels[y][x] = color;
        }
    }
}

void Window::DrawBox(uint16_t x, uint16_t y, const Pixel& color)
{
    DrawBox(x, y, x + 16, y + 16, color);
}

static double degToRad(const double d) { return (d * M_PI / 180); }

void Window::FillRecursive(const uint16_t x, const uint16_t y, const Pixel oldColor, const Pixel& color, const uint16_t c)
{
    if (c == 0)
    {
        return;
    }
    if (pixels[y][x] == oldColor)
    {
        uint16_t sx = x;
        while (x > 0 && pixels[y][sx] == oldColor)
        {
            pixels[y][sx] = color;
            if (y > 0)
            {
                FillRecursive(x, y - 1, oldColor, color, c - 1);
            }
            sx--;
        }

        sx = x + 1;
        while (x < width && pixels[y][sx] == oldColor)
        {
            pixels[y][sx] = color;
            if (y < heigth - 1)
            {
                FillRecursive(x, y + 1, oldColor, color, c - 1);
            }
            sx++;
        }
    }
    else
    {
        return;
    }
}

void Window::Fill(const uint16_t x, const uint16_t y, const Pixel& color)
{
    FillRecursive(x, y, pixels[y][x], color, 250);
}

void Window::PrintText(const uint16_t x, const uint16_t y, const std::string text)
{
    RenderPrintText(pixels, x, y, colors.red, text);
}

void Window::DrawDropper(const uint16_t x, const uint16_t y, const uint16_t radius, const uint16_t deg, const bool left, const Pixel& color)
{
    DrawCircle(x, y, radius, 36 * 2, colors.orange);
    DrawCircle(x, y, radius + 32, 36 * 2, colors.orange);

    double         rad    = degToRad(left ? deg : deg + 180);
    double         sine   = sin(rad);
    double         cosine = cos(rad);
    const uint16_t sx     = x + sine * radius;
    const uint16_t sy     = y + cosine * radius;

    DrawLine(x, y, sx, sy, color);

    static const uint16_t space      = 25;
    static const uint16_t turnOffset = 10;

    for (int i = 0; i < 14; i++)
    {
        uint16_t deg2 = (i % 7) * space + turnOffset;
        if (i > 6)
        {
            deg2 += 180;
        }
        rad    = degToRad(deg2);
        sine   = sin(rad);
        cosine = cos(rad);
        //    DrawDropPoint(x + sine * (radius + 16), y + cosine * (radius + 16), i
        //    + 1,
        //                  color);
    }
}

void Window::DrawCircle(const uint16_t x, const uint16_t y, const uint16_t radius, const uint16_t numberOfSides, const Pixel& color, bool fill)
{
    const double   step   = 360.0 / numberOfSides;
    const uint16_t startx = x;
    const uint16_t starty = y + radius;
    uint16_t       prevx  = startx;
    uint16_t       prevy  = starty;

    for (uint16_t s = 1; s < numberOfSides; s++)
    {
        const double   rad    = degToRad(s * step);
        const double   sine   = sin(rad);
        const double   cosine = cos(rad);
        const uint16_t sx     = x + sine * radius;
        const uint16_t sy     = y + cosine * radius;
        DrawLine(prevx, prevy, sx, sy, color);
        prevx = sx;
        prevy = sy;
    }
    DrawLine(prevx, prevy, startx, starty, color);

    if (fill)
    {
        Fill(x, y, color);
    }
}

void Window::Draw(void)
{
    Clear();

    DrawDropper(width - 150, 150, 100, 100, true, colors.red);
    RenderPrintText(pixels, 10, 10, colors.red, "NO SCreen!");
}

void Window::Open(void)
{
    thread = std::thread(&Window::Thread, this);
}

Pixel::Pixel() :
    r(0), g(0), b(0) {}

Pixel::Pixel(GLubyte r, GLubyte g, GLubyte b) :
    r(r), g(g), b(b) {}
