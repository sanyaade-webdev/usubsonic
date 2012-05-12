TEMPLATE = lib
TARGET = subsonic-quick
QT += declarative network
CONFIG += qt plugin

TARGET = $$qtLibraryTarget($$TARGET)
uri = org.subsonic

INCLUDEPATH += ../libsubsonic
LIBS += -L../libsubsonic -lsubsonic

CONFIG += mobility

MOBILITY += multimedia

# Input
SOURCES += \
    subsonic-quick_plugin.cpp \
    subsonicmediaplayer.cpp

HEADERS += \
    subsonic-quick_plugin.h \
    subsonicmediaplayer.h

OTHER_FILES = qmldir

qmldir.files = qmldir

installPath = $$[QT_INSTALL_IMPORTS]/$$replace(uri, \\., /)

qmldir.path = $$installPath
target.path = $$installPath
INSTALLS += target qmldir

