#include "qfireengine.h"
#include "../src/fire_engine.h"

#include <unistd.h>

#include <QOpenGLFramebufferObjectFormat>
#include <QDebug>

class QFireEngineRenderer : public  QQuickFramebufferObject::Renderer
{
private:
    GLuint m_iTexIndex;
    QSize m_size;
    QOpenGLFramebufferObject *m_fbObj = nullptr;
    FireEngine *m_fireEngine = nullptr;
    std::vector<unsigned int> m_pixels;
    std::vector<unsigned int> m_pixelsNew;
    std::vector<unsigned int> m_pixelsPostProcessed;
    unsigned char m_intensity = 0xFF;
    unsigned char m_channelRed = 0xFF;
    unsigned char m_channelGreen = 0;
    unsigned char m_channelBlue = 0;
    bool m_upsideDown = false;

protected:

    QOpenGLFramebufferObject *createFramebufferObject(const QSize &size) override
    {
        QOpenGLFramebufferObjectFormat format;
        format.setAttachment(QOpenGLFramebufferObject::NoAttachment);
        //Take care of resizing the engine if the FBO is resized
        if (m_fireEngine)
            delete m_fireEngine;
        m_fireEngine = new FireEngine(size.width(), size.height());
        //FBO is managed by Qt
        m_fbObj = new QOpenGLFramebufferObject(size, format);
        m_pixels.resize(size.width()*size.height());
        m_pixelsNew.resize(size.width()*size.height());
        m_pixelsPostProcessed.resize(size.width()*size.height());
        //ZZap with opacity
        memset(&m_pixels[0], 0xFF000000, size.width()*size.height());
        memset(&m_pixelsNew[0], 0xFF000000, size.width()*size.height());
        memset(&m_pixelsPostProcessed[0], 0, size.width()*size.height());
        //New fb obj, new texture
        m_iTexIndex = m_fbObj->texture();
        m_size = size;
        return m_fbObj;
    }


    void synchronize (QQuickFramebufferObject * _item) override {
        QFireEngine* item = static_cast<QFireEngine*>(_item);
        m_intensity = item->getIntensity();
        m_channelRed = item->getChannelRed();
        m_channelBlue = item->getChannelBlue();
        m_channelGreen = item->getChannelGreen();
        m_upsideDown = item->getUpsideDown();
    }


    void render() override
    {
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        m_fireEngine->Scroll(&m_pixels[0], 1);
        m_fireEngine->Ignite(&m_pixels[0], m_size.height() - 1, m_channelRed, m_channelGreen, m_channelBlue, m_intensity);
        m_fireEngine->Burn(&m_pixels[0], &m_pixelsNew[0], 1, m_intensity, 1);
        if (m_upsideDown)
            m_fireEngine->PostProcess(&m_pixelsNew[0], &m_pixelsPostProcessed[0]);
        //This is a bit simple, and could be optimized. But for now will do!
        m_pixels = m_pixelsNew;

        //Very simple old-school OpenGL way of blasting out a texture. First thing that comes to mind, really, and straightforward.
        glBindTexture (GL_TEXTURE_2D, m_fbObj->texture());
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glEnable(GL_BLEND);
        glEnable(GL_TEXTURE_2D);
        //Lets use the simple texture we get from the fb object kthxbye
        if (m_upsideDown)
            glTexImage2D (GL_TEXTURE_2D, 0, GL_RGBA, m_size.width(), m_size.height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, &m_pixelsPostProcessed[0]);
        else
            glTexImage2D (GL_TEXTURE_2D, 0, GL_RGBA, m_size.width(), m_size.height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, &m_pixels[0]);
        glBegin (GL_QUADS);
        glTexCoord2f (0, 0);
        glVertex2f (0, 0);
        glTexCoord2f (m_size.width(), 0);
        glVertex2f (m_size.width(), 0);
        glTexCoord2f (m_size.width(), m_size.height());
        glVertex2f (m_size.width(), m_size.height());
        glTexCoord2f (0, m_size.height());
        glVertex2f (0, m_size.height());
        glEnd();
        //Again please
        update();
    }

    ~QFireEngineRenderer (){
        delete m_fireEngine;
        //Fbo taken care of by qt
    }
};

QFireEngine::QFireEngine() {
    m_intensity = 0xFF;
    m_channelRed = 0xFF;
    m_channelGreen = 0xFF;
    m_channelBlue = 0xFF;
}

QQuickFramebufferObject::Renderer *QFireEngine::createRenderer() const
{
    return new QFireEngineRenderer();
}
