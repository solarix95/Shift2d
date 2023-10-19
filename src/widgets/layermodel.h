#ifndef LAYERMODEL_H
#define LAYERMODEL_H

#include <QAbstractTableModel>
#include "render/renderer.h"

class LayerModel : public QAbstractTableModel
{
    Q_OBJECT
public:
    explicit LayerModel(Renderer &renderer, QObject *parent = 0);
    
    virtual int rowCount(const QModelIndex &index= QModelIndex()) const;
    virtual int columnCount(const QModelIndex &index= QModelIndex()) const;
    virtual QVariant data(const QModelIndex &index, int role) const;
    virtual QVariant headerData(int section, Qt::Orientation orientation, int role) const;

    
private slots:
    void aboutToAddLayer(int index);
    void aboutToRemoveLayer(int index);
    void layerAdded();
    void layerRemoved();
    void layerChanged();
    void beginRendererReset();
    void endRendererReset();
    
private:
    Renderer &mRenderer;

};


#endif // LAYERMODEL_H
