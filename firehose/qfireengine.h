#pragma once
#include <QObject>
#include <QQuickFramebufferObject>


class QFireEngine : public QQuickFramebufferObject
{
    Q_OBJECT
private:    
    QQuickFramebufferObject::Renderer *createRenderer() const override;
};
