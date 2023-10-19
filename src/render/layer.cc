#include <QFileInfo>
#include "render/layer.h"

//-----------------------------------------------------------------------------
Layer::Layer(QObject *parent) :
    QObject(parent)
{
    mShift = 0;
    mScale = 1;
    mMoveX = 0;
    mMoveY = 0;
}

//-----------------------------------------------------------------------------
bool Layer::load(Layer::Source s, const QString &sourceName)
{
    QImage i;
    i.load(sourceName);
    bool both = mLeftSource == mRightSource;

    if (s == Left) {
        mLeftSource = sourceName;
        mLeftImage  = i;      
    }
    if (s == Right || both) {
        mRightSource = sourceName;
        mRightImage  = i;
    }

    emit changed(this);
    return !i.isNull();
}

//-----------------------------------------------------------------------------
void Layer::setShift(int dx)
{
    if (mShift == dx)
        return;

    mShift = dx;
    emit changed(this);
}

//-----------------------------------------------------------------------------
int Layer::shift() const
{
    return mShift;
}

//-----------------------------------------------------------------------------
void Layer::setScale(double s)
{
    if (mScale == s)
        return;

    mScale = s;
    emit changed(this);
}

//-----------------------------------------------------------------------------
double Layer::scale() const
{
    return mScale;
}

//-----------------------------------------------------------------------------
void Layer::setMoveX(int dx)
{
    if (mMoveX == dx)
        return;

    mMoveX = dx;
    emit changed(this);
}

//-----------------------------------------------------------------------------
void Layer::setMoveY(int dy)
{
    if (mMoveY == dy)
        return;

    mMoveY = dy;
    emit changed(this);
}

//-----------------------------------------------------------------------------
int Layer::moveX() const
{
    return mMoveX;
}

//-----------------------------------------------------------------------------
int Layer::moveY() const
{
    return mMoveY;
}

//-----------------------------------------------------------------------------
QImage Layer::render(Layer::Source s) const
{
    QImage sourceImage;
    sourceImage.load(s == Left ? mLeftSource : mRightSource);
    int targetWidth  = sourceImage.width() * mScale;
    int targetHeight = sourceImage.height() * mScale;
    if (targetWidth != sourceImage.width())
        sourceImage = sourceImage.scaled(targetWidth,targetHeight,Qt::IgnoreAspectRatio,Qt::SmoothTransformation);

    int width = sourceImage.width()-abs(mShift);
    QImage i(width,sourceImage.height(),QImage::Format_ARGB32);

    int offset = abs(s == Left ? qMax(mShift,0) : qMin(0,mShift));

    for (int x=0; x<width; x++)
        for (int y=0; y<i.height(); y++)
            i.setPixel(x,y,sourceImage.pixel(x+offset,y));

    return i;
}

//-----------------------------------------------------------------------------
QString Layer::sourceName(Layer::Source s) const
{
    return s == Left ? mLeftSource : mRightSource;
}

//-----------------------------------------------------------------------------
QString Layer::displayName(Source s) const
{
    return QFileInfo(sourceName(s)).fileName();
}

//-----------------------------------------------------------------------------
void Layer::saveState(QSettings &initFile) const
{
    initFile.setValue("left", mLeftSource);
    initFile.setValue("right",mRightSource);
    initFile.setValue("shift",mShift);
    initFile.setValue("scale",mScale);
    initFile.setValue("moveX",mMoveX);
    initFile.setValue("moveY",mMoveY);
}

//-----------------------------------------------------------------------------
void Layer::restoreState(QSettings &initFile)
{
    mLeftSource  = initFile.value("left").toString();
    mRightSource = initFile.value("right").toString();
    mShift       = initFile.value("shift").toInt();
    mScale       = initFile.value("scale").toDouble();
    mMoveX       = initFile.value("moveX").toInt();
    mMoveY       = initFile.value("moveY").toInt();
}
