#ifndef PLAYERWIDGET_H
#define PLAYERWIDGET_H

#include <QWidget>
#include <QFrame>
#include <QSettings>

namespace Ui {
class PlayerForm;
}

class PlayerWidget : public QWidget
{
    Q_OBJECT
    
public:
    explicit PlayerWidget(QWidget *parent = 0);
    virtual ~PlayerWidget();
    
    void store(QSettings *s);
    void restore(QSettings *s);

protected:
    virtual void keyPressEvent(QKeyEvent *);

private slots:
    void frameRendered(const QImage&);
    void saveFrame();
    void updateTheatre();

private:
    Ui::PlayerForm *ui;
    QImage          mLastFrame;
    QString         nLastOutput;
};

#endif // PLAYERWIDGET_H
