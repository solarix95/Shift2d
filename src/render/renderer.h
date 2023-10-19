#ifndef RENDERER_H
#define RENDERER_H

#include <QObject>
#include <QList>
#include <QImage>
#include <QColor>
#include "anaglyph.h"
#include <QSettings>

class Layer;
class Renderer : public QObject
{
    Q_OBJECT
public:
    explicit Renderer(QObject *parent = 0);
    virtual ~Renderer();

    void   addLayer(Layer *layer, int index = -1);
    void   removeLayer(int index);
    void   setCurrentLayer(int index);

    Layer *currentLayer();
    int    layerCount() const;
    Layer *layer(int index) const;

    QImage render(int width, int height, Anaglyph::Type t);

    void   saveState(QSettings &initFile) const;
    void   restoreState(QSettings &initFile);

signals:
    void currentChanged();
    void aboutToAddLayer(int index);
    void aboutToRemoveLayer(int index);
    void layerAdded();
    void layerRemoved();
    void layerChanged(Layer *);
    void beginReset();
    void endReset();
    
public slots:

private:
    QList<Layer*> mLayers;
    int           mCurrentIndex;
};

#endif // RENDERER_H
