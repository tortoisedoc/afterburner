CONFIG -= qtquickcompiler
QT += qml quick core quickcontrols2

HEADERS += ../src/fire_engine.h \
    qfireengine.h
SOURCES += ../src/fire_engine.cpp \
    main.cpp \
    qfireengine.cpp

DISTFILES += \
    main.qml

RESOURCES += \
    main.qrc
