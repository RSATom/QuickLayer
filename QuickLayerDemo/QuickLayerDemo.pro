QT += core gui widgets

TARGET = QuickLayerDemo
TEMPLATE = app

SOURCES += \
    main.cpp \
    MainWidget.cpp

HEADERS += \
    MainWidget.h

LIBS += -framework Cocoa -framework QuartzCore

include( ../QuickLayer.pri )
