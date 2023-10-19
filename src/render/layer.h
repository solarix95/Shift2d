#ifndef LAYER_H
#define LAYER_H

#include <QObject>
#include <QString>
#include <QImage>
#include <QSettings>

class Layer : public QObject
{
    Q_OBJECT
public:
    explicit Layer(QObject *parent = 0);
    
    enum Source {
        Right,
        Left
    };

    bool   load(Source s, const QString &sourceName);
    void   setShift(int dx);
    int    shift() const;
    void   setScale(double s);
    double scale() const;
    void   setMoveX(int dx);
    void   setMoveY(int dy);
    int    moveX() const;
    int    moveY() const;
    QImage render(Source s) const;
    QString sourceName(Source s) const;
    QString displayName(Source s) const;

    void   saveState(QSettings &initFile) const;
    void   restoreState(QSettings &initFile);

signals:
    void changed(Layer*);

private:
    QString mLeftSource;
    QString mRightSource;
    QImage  mLeftImage;
    QImage  mRightImage;
    int     mShift;
    double  mScale;
    int     mMoveX;
    int     mMoveY;
};

#endif // LAYER_H
