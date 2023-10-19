#include <QApplication>
#include <QFileDialog>
#include "widgets/layerwidget.h"
#include "widgets/layermodel.h"
#include "ui_layerwidgetform.h"

#include "render/layer.h"

//-------------------------------------------------------------------------------------------------
LayerWidget::LayerWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::LayerWidgetForm)
{
    ui->setupUi(this);

    Layer *l = new Layer();
    l->load(Layer::Left, "background2.png");
    l->load(Layer::Right,"background2.png");
    l->setShift(10);
    mRenderer.addLayer(l);

    connect(ui->btnRender,      SIGNAL(clicked()),
            this, SLOT(render()));
    connect(ui->sbpShift,       SIGNAL(valueChanged(int)),
            this, SLOT(newShift(int)));
    connect(ui->spbScale,       SIGNAL(valueChanged(double)),
            this, SLOT(newScale(double)));
    connect(ui->btnLeftImage,   SIGNAL(clicked()),
            this, SLOT(selectLeft()));
    connect(ui->btnRightImage,  SIGNAL(clicked()),
            this, SLOT(selectRight()));
    connect(ui->btnAddLayer,    SIGNAL(clicked()),
            this, SLOT(addLayer()));
    connect(ui->btnRemoveLayer, SIGNAL(clicked()),
            this, SLOT(removeLayer()));
    connect(ui->spbMoveX, SIGNAL(valueChanged(int)),
            this, SLOT(newMoveX()));
    connect(ui->spbMoveY, SIGNAL(valueChanged(int)),
            this, SLOT(newMoveY()));
    connect(ui->btnSave, SIGNAL(clicked(bool)),
            this, SLOT(saveProject()));
    connect(ui->btnSaveAs, SIGNAL(clicked(bool)),
            this, SLOT(saveAsProject()));
    connect(ui->btnOpen, SIGNAL(clicked(bool)),
            this, SLOT(openProject()));


    connect(&mRenderer,SIGNAL(currentChanged()), this, SLOT(updateWidgets()));
    connect(&mRenderer,SIGNAL(layerChanged(Layer*)), this, SLOT(updateWidgets()));
    connect(&mRenderer, SIGNAL(endReset()), this, SLOT(updateWidgets()));

    ui->layerView->setModel(new LayerModel(mRenderer));
    connect(ui->layerView->selectionModel(), SIGNAL(currentChanged(QModelIndex,QModelIndex)), this, SLOT(updateLayer()));
}

//-------------------------------------------------------------------------------------------------
LayerWidget::~LayerWidget()
{
    delete ui;
}

//-------------------------------------------------------------------------------------------------
void LayerWidget::store(QSettings *s)
{
    s->beginGroup("LayerWidget");
    s->setValue("Geometry",geometry());
    s->setValue("Pos", pos());
    s->setValue("RenderWidth",ui->spbWidth->value());
    s->setValue("RenderHeight",ui->spbHeight->value());
    s->setValue("RenderFormat",ui->cbxFormat->currentIndex());
    s->setValue("LastInput",mLastInput);
    s->endGroup();
}

//-------------------------------------------------------------------------------------------------
void LayerWidget::restore(QSettings *s)
{
    s->beginGroup("LayerWidget");
    if (s->contains("Geometry"))
        setGeometry(s->value("Geometry").toRect());
    if (s->contains("Pos"))
        move(s->value("Pos").toPoint());
    if (s->contains("RenderWidth"))
        ui->spbWidth->setValue(s->value("RenderWidth").toInt());
    if (s->contains("RenderHeight"))
        ui->spbHeight->setValue(s->value("RenderHeight").toInt());
    if (s->contains("RenderFormat"))
        ui->cbxFormat->setCurrentIndex(s->value("RenderFormat").toInt());

    mLastInput = s->value("LastInput").toString();
    s->endGroup();
}

//-------------------------------------------------------------------------------------------------
void LayerWidget::render()
{
    QApplication::setOverrideCursor(QCursor(Qt::WaitCursor));
    while (QCoreApplication::hasPendingEvents())
        QCoreApplication::processEvents();

     emit rendered(mRenderer.render(ui->spbWidth->value(),ui->spbHeight->value(),(Anaglyph::Type)(ui->cbxFormat->currentIndex())));

    QApplication::restoreOverrideCursor();


}

//-------------------------------------------------------------------------------------------------
void LayerWidget::selectLeft()
{
    QString fname = QFileDialog::getOpenFileName(this,"Left Source",mLastInput);
    if (fname.isEmpty())
        return;
    mLastInput = fname;
    mRenderer.currentLayer()->load(Layer::Left,mLastInput);
}

//-------------------------------------------------------------------------------------------------
void LayerWidget::selectRight()
{
    QString fname = QFileDialog::getOpenFileName(this,"Right Source",mLastInput);
    if (fname.isEmpty())
        return;
    mLastInput = fname;
    mRenderer.currentLayer()->load(Layer::Right,mLastInput);
}

//-------------------------------------------------------------------------------------------------
void LayerWidget::addLayer()
{
    int row = ui->layerView->currentIndex().row();
    mRenderer.addLayer(new Layer(), row);
}

//-------------------------------------------------------------------------------------------------
void LayerWidget::removeLayer()
{
    int row = ui->layerView->currentIndex().row();
    mRenderer.removeLayer(row);
}

//-------------------------------------------------------------------------------------------------
void LayerWidget::saveProject()
{
    if (mProjectName.isEmpty()) {
        saveAsProject();
        return;
    }

    QSettings project(mProjectName, QSettings::IniFormat);
    mRenderer.saveState(project);
    project.sync();
}

//-------------------------------------------------------------------------------------------------
void LayerWidget::openProject()
{
    QString openProjectName = QFileDialog::getOpenFileName(this, tr("Open Shift-Project"),mLastInput, "Shift3 (*.sft *.ini)");
    if (openProjectName.isEmpty())
        return;

    mProjectName = openProjectName;
    mLastInput   = openProjectName;
    QSettings project(mProjectName, QSettings::IniFormat);
    mRenderer.restoreState(project);
    ui->lblProjectName->setText(mProjectName);
}

//-------------------------------------------------------------------------------------------------
void LayerWidget::saveAsProject()
{
    QString newProjectName = QFileDialog::getSaveFileName(this, tr("Save Shift-Project"),mLastInput, "Shift3 (*.sft *.ini)");
    if (newProjectName.isEmpty())
        return;

    mProjectName = newProjectName;
    mLastInput   = newProjectName;
    if (!mProjectName.contains("."))
        mProjectName.append(".sft");

    QSettings project(mProjectName, QSettings::IniFormat);
    mRenderer.saveState(project);
    project.sync();
    ui->lblProjectName->setText(mProjectName);
}

//-------------------------------------------------------------------------------------------------
void LayerWidget::newShift(int s)
{
    if (mRenderer.currentLayer())
        mRenderer.currentLayer()->setShift(s);
}

//-------------------------------------------------------------------------------------------------
void LayerWidget::newScale(double s)
{
    if (mRenderer.currentLayer())
        mRenderer.currentLayer()->setScale(s);
}

//-------------------------------------------------------------------------------------------------
void LayerWidget::newMoveX()
{
    if (mRenderer.currentLayer())
        mRenderer.currentLayer()->setMoveX(ui->spbMoveX->value());
}

//-------------------------------------------------------------------------------------------------
void LayerWidget::newMoveY()
{
    if (mRenderer.currentLayer())
        mRenderer.currentLayer()->setMoveY(ui->spbMoveY->value());
}

//-------------------------------------------------------------------------------------------------
void LayerWidget::closeEvent(QCloseEvent *e)
{
    qApp->quit();
    QWidget::closeEvent(e);
}

//-------------------------------------------------------------------------------------------------
void LayerWidget::updateWidgets()
{
    Layer *l = mRenderer.currentLayer();
    ui->btnLeftImage->setEnabled(l ? true:false);
    ui->btnRightImage->setEnabled(l ? true:false);
    ui->sbpShift->setEnabled(l ? true:false);

    ui->lblLeftImage->setText(l ? l->sourceName(Layer::Left) : QString());
    ui->lblRightImage->setText(l ? l->sourceName(Layer::Right) : QString());
    ui->sbpShift->setValue(l ? l->shift() : 0);
    ui->spbScale->setValue(l ? l->scale() : 0);

    ui->spbMoveX->setValue(l ? l->moveX() : 0);
    ui->spbMoveY->setValue(l ? l->moveY() : 0);
}

//-------------------------------------------------------------------------------------------------
void LayerWidget::updateLayer()
{
    mRenderer.setCurrentLayer(ui->layerView->currentIndex().row());
}
