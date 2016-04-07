#-------------------------------------------------
#
# Project created by QtCreator 2015-08-02T01:57:21
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

QMAKE_CXXFLAGS+= -std=c++0x -pthread
QMAKE_LFLAGS +=  -std=c++0x -pthread

TARGET = player
TEMPLATE = app
LIBS += -lvlc -lglut -lGL -lGLU -lGLEW
QT += opengl
SOURCES += \
    core/main.cpp \
    core/Instance.cpp \
    core/Mediaplayer.cpp \
    core/Interface.cpp \
    core/Player.cpp \
    core/Video.cpp \
    core/Media.cpp \
    core/Audio.cpp \
    widgets/VideoWindow.cpp \
    core/DelegateControl.cpp \
    core/Filters.cpp \
    core/EventsEmitter.cpp \
    widgets/ControlPanel.cpp \
    core/VideoShow.cpp \
    core/ViewThread.cpp \
    core/Anaglyph.cpp

HEADERS  += \
    core/Instance.h \
    core/Mediaplayer.h \
    core/Interface.h \
    core/Player.h \
    core/Video.h \
    core/Media.h \
    core/Audio.h \
    widgets/VideoSlider.h \
    widgets/VideoWindow.h \
    core/DelegateControl.h \
    core/Filters.h \
    core/EventsEmitter.h \
    core/Options.h \
    widgets/ControlPanel.h \
    core/VideoShow.h \
    core/ViewThread.h \
    core/Anaglyph.h \
    core/Shaders.h

FORMS    += forms/player.ui

RESOURCES += \


