
#include <QDebug>
#include <QApplication>
#include "widgets/playerwidget.h"
#include "widgets/layerwidget.h"
#include <QSettings>

//-------------------------------------------------------------------------------------------------
int main(int argc, char *argv[])
{
    QApplication app(argc,argv);

    QSettings settings("HELD", "SHIFT4");

    //--------------------------------------------------------------------------------------------
    PlayerWidget player;
    player.restore(&settings);
    player.show();

    LayerWidget layerWidget;
    layerWidget.restore(&settings);
    layerWidget.show();

    QObject::connect(&layerWidget, SIGNAL(rendered(const QImage&)), &player, SLOT(frameRendered(const QImage&)));

    //--------------------------------------------------------------------------------------------
    int ret = app.exec();

    player.store(&settings);
    layerWidget.store(&settings);

    return ret;
}
