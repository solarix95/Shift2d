#ifndef ANAGLYPH_H
#define ANAGLYPH_H

#include <QColor>

class Anaglyph
{
public:
    enum Type {
        TrueAnaglyph,
        GrayAnaglyph,
        ColorAnaglyph,
        HalfColorAnaglyph,
        OptimizedAnaglyph
    };

    Anaglyph(Type anaglyph);

    QRgb mixer(const QRgb &r, const QRgb &l);

private:
    float mMatrix[3][3];
};

#endif // ANAGLYPH_H
