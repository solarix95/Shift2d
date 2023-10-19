#include "widgets/layermodel.h"
#include "render/layer.h"

//-----------------------------------------------------------------------------
LayerModel::LayerModel(Renderer &renderer, QObject *parent) :
    QAbstractTableModel(parent), mRenderer(renderer)
{
    connect(&mRenderer, SIGNAL(aboutToAddLayer(int)), this, SLOT(aboutToAddLayer(int)));
    connect(&mRenderer, SIGNAL(layerAdded()), this, SLOT(layerAdded()));
    connect(&mRenderer, SIGNAL(aboutToRemoveLayer(int)), this, SLOT(aboutToRemoveLayer(int)));
    connect(&mRenderer, SIGNAL(layerRemoved()), this, SLOT(layerRemoved()));
    connect(&mRenderer, SIGNAL(layerChanged(Layer*)), this, SLOT(layerChanged()));
    connect(&mRenderer, SIGNAL(beginReset()), this, SLOT(beginRendererReset()));
    connect(&mRenderer, SIGNAL(endReset()), this, SLOT(endRendererReset()));
}

//-----------------------------------------------------------------------------
int LayerModel::rowCount(const QModelIndex &) const
{
    return mRenderer.layerCount();
}

//-----------------------------------------------------------------------------
int LayerModel::columnCount(const QModelIndex &) const
{
    return 2;
}

//-----------------------------------------------------------------------------
QVariant LayerModel::data(const QModelIndex &index, int role) const
{
    int r = index.row();
    switch (role) {
    case Qt::DisplayRole: {
        if (index.column() == 0)
            return mRenderer.layer(r)->displayName(Layer::Left);
        else
            return mRenderer.layer(r)->displayName(Layer::Right);
    } break;
    case Qt::ToolTip: {
        if (index.column() == 0)
            return mRenderer.layer(r)->sourceName(Layer::Left);
        else
            return mRenderer.layer(r)->sourceName(Layer::Right);
    } break;
    }

    return QVariant();
}

//-----------------------------------------------------------------------------
QVariant LayerModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (orientation == Qt::Vertical && role == Qt::DisplayRole) {
        if (section == 0 )
            return "top";
        if (section == rowCount()-1)
            return "buttom";
        return QString("%1").arg(section);
    }

    if (orientation == Qt::Horizontal && role == Qt::DisplayRole) {
        if (section == 0)
            return "Left";
        if (section == 1)
            return "Right";
    }

    return QAbstractTableModel::headerData(section,orientation,role);
}

//-----------------------------------------------------------------------------
void LayerModel::aboutToAddLayer(int index)
{
    beginInsertRows(QModelIndex(),index,index);
}

//-----------------------------------------------------------------------------
void LayerModel::aboutToRemoveLayer(int index)
{
    beginRemoveRows(QModelIndex(),index,index);
}

//-----------------------------------------------------------------------------
void LayerModel::layerAdded()
{
    endInsertRows();
}

//-----------------------------------------------------------------------------
void LayerModel::layerRemoved()
{
    endRemoveRows();
}

//-----------------------------------------------------------------------------
void LayerModel::layerChanged()
{
    Q_ASSERT(rowCount() > 0);
    emit dataChanged(index(0,0),index(rowCount()-1,columnCount()-1));
}

//-----------------------------------------------------------------------------
void LayerModel::beginRendererReset()
{
    beginResetModel();
}

//-----------------------------------------------------------------------------
void LayerModel::endRendererReset()
{
    endResetModel();
}
