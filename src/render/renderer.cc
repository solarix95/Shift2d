#include <QPainter>
#include "render/renderer.h"
#include "render/layer.h"
#include "render/anaglyph.h"

//-----------------------------------------------------------------------------
Renderer::Renderer(QObject *parent) :
    QObject(parent)
{
    mCurrentIndex = -1;
}

//-----------------------------------------------------------------------------
Renderer::~Renderer()
{
    qDeleteAll(mLayers);
}

//-----------------------------------------------------------------------------
void Renderer::addLayer(Layer *layer, int index)
{
    if (index < 0) {
        emit aboutToAddLayer(mLayers.count());
        mLayers << layer;
        index = mLayers.count()-1;
    } else {
        emit aboutToAddLayer(index);
        mLayers.insert(index,layer);
        index = qMin(index,mLayers.count()-1);
    }
    emit layerAdded();
    setCurrentLayer(index);
    connect(layer, SIGNAL(changed(Layer*)), this, SIGNAL(layerChanged(Layer*)));
}

//-----------------------------------------------------------------------------
void Renderer::removeLayer(int index)
{
    if (index < 0 || index >= mLayers.count())
        return;

    emit aboutToRemoveLayer(index);

    delete mLayers.takeAt(index);

    if (mCurrentIndex >= mLayers.count())
        setCurrentLayer(-1);

    emit layerRemoved();
}

//-----------------------------------------------------------------------------
void Renderer::setCurrentLayer(int index)
{
    if (mCurrentIndex == index)
        return;
    mCurrentIndex = index;
    emit currentChanged();
}

//-----------------------------------------------------------------------------
Layer *Renderer::currentLayer()
{
    if (mCurrentIndex >= 0 && mCurrentIndex < mLayers.count())
        return mLayers.at(mCurrentIndex);

    return NULL;
}

//-----------------------------------------------------------------------------
int Renderer::layerCount() const
{
    return mLayers.count();
}

//-----------------------------------------------------------------------------
Layer *Renderer::layer(int index) const
{
    Q_ASSERT(index >= 0);
    Q_ASSERT(index < mLayers.count());
    return mLayers[index];
}

//-----------------------------------------------------------------------------
QImage Renderer::render(int width, int height, Anaglyph::Type t)
{
    QImage result(width,height,QImage::Format_ARGB32);
    result.fill(Qt::transparent);

    if (mLayers.isEmpty())
        return result;

    QImage l(width,height,QImage::Format_ARGB32);
    l.fill(Qt::transparent);
    QPainter pl(&l);

    QImage r(width,height,QImage::Format_ARGB32);
    r.fill(Qt::transparent);
    QPainter pr(&r);

    for (int i=mLayers.count()-1; i>= 0; i--) {
        QImage li = mLayers.at(i)->render(Layer::Left);
        QImage ri = mLayers.at(i)->render(Layer::Right);
        Q_ASSERT(li.size() == ri.size());
        int startX = (width-li.width())/2;
        int startY = (height-li.height())/2;
        pl.drawImage(startX + mLayers.at(i)->moveX(),startY + mLayers.at(i)->moveY(),li);
        pr.drawImage(startX + mLayers.at(i)->moveX(),startY + mLayers.at(i)->moveY(),ri);
    }

    /*
        TrueAnaglyph,
        GrayAnaglyph,
        ColorAnaglyph,
        HalfColorAnaglyph,
        OptimizedAnaglyph
    */
    Anaglyph a(t);
    for (int x=0; x<l.width(); x++)
        for (int y=0; y<l.height(); y++)
            result.setPixel(x,y,a.mixer(l.pixel(x,y),r.pixel(x,y)));
    return result;
}

//-----------------------------------------------------------------------------
void Renderer::saveState(QSettings &initFile) const
{
    initFile.clear();
    initFile.beginGroup("Renderer");
    initFile.setValue("layers",mLayers.count());
    initFile.endGroup();

    for (int i=0; i<mLayers.count(); i++) {
        initFile.beginGroup(QString("layer%1").arg(i));
        mLayers[i]->saveState(initFile);
        initFile.endGroup();
    }
}

//-----------------------------------------------------------------------------
void Renderer::restoreState(QSettings &initFile)
{
    emit beginReset();
    qDeleteAll(mLayers);
    mLayers.clear();

    initFile.beginGroup("Renderer");
    int layerCount =  initFile.value("layers").toInt();
    initFile.endGroup();

    for (int i=0; i<layerCount; i++) {
        initFile.beginGroup(QString("layer%1").arg(i));
        Layer *l = new Layer();
        l->restoreState(initFile);
        mLayers.append(l);
        initFile.endGroup();
    }

   mCurrentIndex = 0;
   emit endReset();
}

