CONFIG += c++11

QT += quick

HEADERS += \
    $$PWD/FboQuickWindow.h \
    $$PWD/FboQuickView.h \
    $$PWD/FboQuickWrapperWindow.h

SOURCES += \
    $$PWD/FboQuickWindow.cpp \
    $$PWD/FboQuickView.cpp \
    $$PWD/FboQuickWrapperWindow.cpp

macx {
    HEADERS += \
        $$PWD/QuickLayer.h \

    OBJECTIVE_HEADERS += \
        $$PWD/QuickLayer.h

    OBJECTIVE_SOURCES += \
        $$PWD/QuickLayer.mm

    LIBS += -framework Cocoa -framework QuartzCore

    INCLUDEPATH += $(QTDIR)/include
}

INCLUDEPATH += $$PWD
