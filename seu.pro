TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

CONFIG += debug

SOURCES += \
        Assets.c \
        Bullet.c \
        Camera.c \
        Common.c \
        Enemy.c \
        Input.c \
        Math.c \
        Player.c \
        Scene.c \
        Timer.c \
        levels.c \
        main.c \
        patterns.c \
        uicomponents.c

HEADERS += \
    Assets.h \
    Bullet.h \
    Camera.h \
    Common.h \
    Enemy.h \
    Input.h \
    Math.h \
    Player.h \
    Scene.h \
    Settings.h \
    Timer.h \
    levels.h \
    patterns.h \
    uicomponents.h

LIBS += -lm -lSDL2 -lSDL2_image -lSDL2_mixer -lSDL2_ttf
