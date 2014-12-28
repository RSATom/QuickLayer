QT += core gui widgets

TARGET = QuickLayerDemo
TEMPLATE = app

HEADERS += \
    MainWidget.h

SOURCES += \
    main.cpp

OBJECTIVE_SOURCES += \
    MainWidget.mm

RESOURCES += main.qrc

LIBS += -framework Cocoa -framework QuartzCore

include( ../QuickLayer.pri )
