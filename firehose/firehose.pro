CONFIG -= qtquickcompiler
QT += qml quick core quickcontrols2

INCLUDEPATH+=/usr/include/opencv4
LIBS+=-lopencv_core -lopencv_imgproc

HEADERS += ../src/fire_engine.h \
    qfireengine.h
SOURCES += ../src/fire_engine.cpp \
    main.cpp \
    qfireengine.cpp

DISTFILES += \
    LabeledSlider.qml \
    main.qml

RESOURCES += \
    main.qrc
