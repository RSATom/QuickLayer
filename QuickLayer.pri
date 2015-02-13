CONFIG += c++11

QT += quick

HEADERS += \
    $$PWD/QuickLayer.h \
    $$PWD/FboQuickWindow.h \
    $$PWD/FboQuickView.h

SOURCES += \
    $$PWD/FboQuickWindow.cpp \
    $$PWD/FboQuickView.cpp

OBJECTIVE_HEADERS += $$PWD/QuickLayer.h
OBJECTIVE_SOURCES += $$PWD/QuickLayer.mm

INCLUDEPATH += $(QTDIR)/include

LIBS += -framework Cocoa -framework QuartzCore
