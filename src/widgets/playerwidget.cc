#include <QDebug>
#include <QFileDialog>
#include <QKeyEvent>
#include "playerwidget.h"
#include "ui_playerform.h"

//-------------------------------------------------------------------------------------------------
PlayerWidget::PlayerWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PlayerForm)
{
    ui->setupUi(this);
    connect(ui->btnSave,  SIGNAL(clicked(bool)), this, SLOT(saveFrame()));
    connect(ui->chkAlign, SIGNAL(clicked(bool)), this, SLOT(updateTheatre()));
    connect(ui->chkAlpha, SIGNAL(clicked(bool)), this, SLOT(updateTheatre()));
}

//-------------------------------------------------------------------------------------------------
PlayerWidget::~PlayerWidget()
{
    delete ui;
}

//-------------------------------------------------------------------------------------------------
void PlayerWidget::store(QSettings *s)
{
    s->beginGroup("PlayerWidget");
    s->setValue("Geometry",geometry());
    s->setValue("Pos", pos());
    s->setValue("Align", ui->chkAlign->checkState());
    s->setValue("Alpha", ui->chkAlpha->checkState());
    s->setValue("LastDir", nLastOutput);
    s->endGroup();
}

//-------------------------------------------------------------------------------------------------
void PlayerWidget::restore(QSettings *s)
{
    s->beginGroup("PlayerWidget");
    if (s->contains("Geometry"))
        setGeometry(s->value("Geometry").toRect());
    if (s->contains("Pos"))
        move(s->value("Pos").toPoint());
    if (s->contains("Align"))
        ui->chkAlign->setCheckState((Qt::CheckState)s->value("Align").toInt());
    if (s->contains("Alpha"))
        ui->chkAlpha->setCheckState((Qt::CheckState)s->value("Alpha").toInt());
    nLastOutput = s->value("LastDir").toString();
    s->endGroup();
}

//-------------------------------------------------------------------------------------------------
void PlayerWidget::keyPressEvent(QKeyEvent *e)
{
    if (e->key() == Qt::Key_F11) { // toggle Fullscreen
        Qt::WindowStates s = isFullScreen() ? (windowState() & ~Qt::WindowFullScreen) : (windowState() | Qt::WindowFullScreen);
        setWindowState(s);
    }
    QWidget::keyPressEvent(e);
}

//-------------------------------------------------------------------------------------------------
void PlayerWidget::frameRendered(const QImage &i)
{
    mLastFrame = i;
    updateTheatre();
}

//-------------------------------------------------------------------------------------------------
void PlayerWidget::saveFrame()
{
    if (mLastFrame.isNull())
        return;

    QString fileName = QFileDialog::getSaveFileName(this,"Save Frame",nLastOutput,tr("Images (*.png *.xpm *.jpg)"));
    if (fileName.isEmpty() || mLastFrame.isNull())
        return;
    nLastOutput = fileName;
    mLastFrame.save(fileName);
}

//-------------------------------------------------------------------------------------------------
void PlayerWidget::updateTheatre()
{
    if (mLastFrame.isNull())
        return;

    QApplication::setOverrideCursor(Qt::WaitCursor);
    QSize scaledSize(ui->scrollArea->width()-10,ui->scrollArea->height()-20);

    QImage showImg = ui->chkAlign->isChecked() ? mLastFrame.scaled(scaledSize,Qt::KeepAspectRatio) : mLastFrame;

    QRgb gray     = QColor(Qt::gray).rgb();
    QRgb darkGray = QColor(Qt::darkGray).rgb();

    if (ui->chkAlpha->isChecked()) {
        for (int y=0; y<showImg.height(); y++) {
            for (int x=0; x<showImg.width(); x++) {
                QRgb c = showImg.pixel(x,y);
                if (qAlpha(c) < 255) {
                    float f = qAlpha(c)/255.0;
                    QRgb ac = ((x/10 + (y/10%2)) % 2) ? gray : darkGray;
                    showImg.setPixel(x,y,qRgb(qRed(c) * f   + qRed(ac)*(1-f),
                                              qGreen(c) * f + qGreen(ac)*(1-f),
                                              qBlue(c) * f  + qBlue(ac)*(1-f)));
                }
            }
        }
    }
    ui->lblView->setPixmap(QPixmap::fromImage(showImg));

    QApplication::restoreOverrideCursor();
    show();
    raise();
}
