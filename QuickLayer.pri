QT += quick

HEADERS += \
    $$PWD/QuickLayer.h \
    $$PWD/FboQuickWindow.h

SOURCES += \
    $$PWD/FboQuickWindow.cpp

OBJECTIVE_HEADERS += $$PWD/QuickLayer.h
OBJECTIVE_SOURCES += $$PWD/QuickLayer.mm

INCLUDEPATH += $(QTDIR)/include

LIBS += -framework Cocoa -framework QuartzCore
