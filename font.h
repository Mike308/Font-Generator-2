#ifndef FONT_H
#define FONT_H

class Font
{
public:
    Font();
    Font(char c, int width, int height, int x, int y);

    int getWidth() const;
    int getHeight() const;
    int getX() const;
    int getY() const;
    char getC() const;

private:
    int width, height, x, y;
    char c;

};

#endif // FONT_H
