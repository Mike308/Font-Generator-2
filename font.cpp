#include "font.h"

Font::Font() {}

Font::Font(char c, int width, int height, int x, int y)
{
    this->c = c;
    this->width = width;
    this->height = height;
    this->x = x;
    this->y = y;
}

int Font::getWidth() const
{
    return width;
}

int Font::getHeight() const {
    return height;
}

int Font::getX() const {
    return x;
}

int Font::getY() const {
    return y;
}


char Font::getC() const {
    return c;
}
