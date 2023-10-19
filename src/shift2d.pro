TEMPLATE = app
TARGET   = shift2d 
DEPENDPATH += .
CONFIG += debug
QT    += widgets

# Input
SOURCES += main.cc \
    widgets/playerwidget.cc \
    widgets/layerwidget.cc \
    render/renderer.cc \
    render/layer.cc \
    render/anaglyph.cc \
    widgets/layermodel.cc

HEADERS += \
    widgets/playerwidget.h \
    widgets/layerwidget.h \
    render/renderer.h \
    render/layer.h \
    render/anaglyph.h \
    widgets/layermodel.h

FORMS += \
    widgets/playerform.ui \
    render.ui \
    widgets/layerwidgetform.ui

RESOURCES += \
    shift2d.qrc
