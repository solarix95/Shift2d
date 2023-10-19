#ifndef LAYERWIDGET_H
#define LAYERWIDGET_H

#include <QWidget>
#include <QSettings>
#include "render/renderer.h"

namespace Ui {
class LayerWidgetForm;
}

class LayerWidget : public QWidget
{
    Q_OBJECT
public:
    explicit LayerWidget(QWidget *parent = 0);
    virtual ~LayerWidget();
    
    void store(QSettings *s);
    void restore(QSettings *s);

signals:
    void rendered(const QImage &i);

protected:
    virtual void closeEvent(QCloseEvent *);

private slots:
    void updateWidgets();
    void updateLayer();
    void newShift(int s);
    void newScale(double s);
    void newMoveX();
    void newMoveY();
    void render();
    void selectLeft();
    void selectRight();
    void addLayer();
    void removeLayer();

    void saveProject();
    void openProject();
    void saveAsProject();

private:
    Ui::LayerWidgetForm *ui;

    Renderer mRenderer;
    QString  mLastInput;
    QString  mProjectName;
};

#endif // LAYERWIDGET_H
