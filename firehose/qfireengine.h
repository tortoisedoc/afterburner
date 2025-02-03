#pragma once
#include <QObject>
#include <QQuickFramebufferObject>


class QFireEngine : public QQuickFramebufferObject
{
    Q_OBJECT
    QML_ELEMENT
    Q_PROPERTY(unsigned char intensity READ getIntensity WRITE setIntensity NOTIFY intensityChanged)
    Q_PROPERTY(unsigned char redChannel READ getChannelRed WRITE setChannelRed NOTIFY redChannelChanged)
    Q_PROPERTY(unsigned char greenChannel READ getChannelGreen WRITE setChannelGreen NOTIFY greenChannelChanged)
    Q_PROPERTY(unsigned char blueChannel READ getChannelBlue WRITE setChannelBlue NOTIFY blueChannelChanged)

public:
    QFireEngine();

    unsigned char getIntensity() {
        return m_intensity;
    }

    void setIntensity(unsigned char value) {
        m_intensity = value;
        emit intensityChanged();
        update();
    }

    unsigned char getChannelRed() {
        return m_channelRed;
    }

    void setChannelRed(unsigned char value) {
        m_channelRed = value;
        emit redChannelChanged();
        update();
    }

    unsigned char getChannelGreen() {
        return m_channelGreen;
    }

    void setChannelGreen(unsigned char value) {
        m_channelGreen = value;
        emit greenChannelChanged();
        update();
    }

    unsigned char getChannelBlue() {
        return m_channelBlue;
    }

    void setChannelBlue(unsigned char value) {
        m_channelBlue = value;
        emit blueChannelChanged();
        update();
    }

signals:
    void intensityChanged();
    void redChannelChanged();
    void greenChannelChanged();
    void blueChannelChanged();
protected:
    QQuickFramebufferObject::Renderer *createRenderer() const override;
private:
    unsigned char m_intensity;
    unsigned char m_channelRed;
    unsigned char m_channelGreen;
    unsigned char m_channelBlue;
};
