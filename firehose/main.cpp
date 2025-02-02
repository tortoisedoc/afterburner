#include <stdio.h>
#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <qqml.h>
#include "qfireengine.h"

int main (int argc, char** argv) {
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QGuiApplication app(argc, argv);
    QQmlApplicationEngine engine;

    qmlRegisterType<QFireEngine>("FireEngine", 1, 0, "FireEngine");
    QUrl url("qrc:/main.qml");
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
        &app, [url](QObject *obj, const QUrl &objUrl) {
            if (!obj && url == objUrl) {
                qDebug() << " COULD NOT LOAD MAIL QML FILE " << url <<"; EXITING";
                QCoreApplication::exit(-1);
            }
        }, Qt::QueuedConnection);
    engine.load(url);
    app.exec();
}
