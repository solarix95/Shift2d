#include "render/anaglyph.h"

Anaglyph::Anaglyph(Anaglyph::Type anaglyph)
{
    // http://www.3dtv.at/knowhow/anaglyphcomparison_en.aspx
    switch (anaglyph) {
    case TrueAnaglyph:
        mMatrix[0][0] = 0.299;
        mMatrix[1][0] = 0.587;
        mMatrix[2][0] = 0.114;
        mMatrix[0][1] = 0;
        mMatrix[1][1] = 0;
        mMatrix[2][1] = 0;
        mMatrix[0][2] = 0.299;
        mMatrix[1][2] = 0.587;
        mMatrix[2][2] = 0.114;
        break;
    case GrayAnaglyph:
        mMatrix[0][0] = 0.299;
        mMatrix[1][0] = 0.587;
        mMatrix[2][0] = 0.114;
        mMatrix[0][1] = 0.299;
        mMatrix[1][1] = 0.587;
        mMatrix[2][1] = 0.114;
        mMatrix[0][2] = 0.299;
        mMatrix[1][2] = 0.587;
        mMatrix[2][2] = 0.114;
        break;
    case ColorAnaglyph:
        mMatrix[0][0] = 1;
        mMatrix[1][0] = 0;
        mMatrix[2][0] = 0;
        mMatrix[0][1] = 0;
        mMatrix[1][1] = 1;
        mMatrix[2][1] = 0;
        mMatrix[0][2] = 0;
        mMatrix[1][2] = 0;
        mMatrix[2][2] = 1;
        break;
    case HalfColorAnaglyph:
        mMatrix[0][0] = 0.299;
        mMatrix[1][0] = 0.587;
        mMatrix[2][0] = 0.114;
        mMatrix[0][1] = 0;
        mMatrix[1][1] = 1;
        mMatrix[2][1] = 0;
        mMatrix[0][2] = 0;
        mMatrix[1][2] = 0;
        mMatrix[2][2] = 1;
        break;
    case OptimizedAnaglyph:
        mMatrix[0][0] = 0;
        mMatrix[1][0] = 0.7;
        mMatrix[2][0] = 0.3;
        mMatrix[0][1] = 0;
        mMatrix[1][1] = 1;
        mMatrix[2][1] = 0;
        mMatrix[0][2] = 0;
        mMatrix[1][2] = 0;
        mMatrix[2][2] = 1;
        break;
    }
}

QRgb Anaglyph::mixer(const QRgb &r, const QRgb &l)
{
    return qRgba(mMatrix[0][0]*qRed(r) + mMatrix[1][0]*qGreen(r) + mMatrix[2][0]*qBlue(r),
                 mMatrix[0][1]*qRed(l) + mMatrix[1][1]*qGreen(l) + mMatrix[2][1]*qBlue(l),
                 mMatrix[0][2]*qRed(l) + mMatrix[1][2]*qGreen(l) + mMatrix[2][2]*qBlue(l),
                 qMax(qAlpha(r),qAlpha(l)));

}


