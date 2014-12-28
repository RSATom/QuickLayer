#pragma once

#include <QQuickWindow>
#include <QQuickRenderControl>
#include <QTimer>

QT_FORWARD_DECLARE_CLASS( QOffscreenSurface )

struct RenderControlHolder
{
    QQuickRenderControl m_renderControl;
};

class FboQuickWindow : private RenderControlHolder, public QQuickWindow
{
public:
    //takes opnership over passed QOpenGLContext
    FboQuickWindow( QOpenGLContext* = 0 );
    ~FboQuickWindow();

    QOpenGLFramebufferObject* fbo() const
        { return m_fbo; }

    //takes opnership over passed QOpenGLContext
    void init( QOpenGLContext* );

protected:
    void resizeEvent( QResizeEvent* );

private Q_SLOTS:
    void sceneGraphInitialized();
    void sceneGraphInvalidated();

    void renderRequested();
    void sceneChanged();

    void render();
    void fullRender();

private:
    void createFbo();
    void destroyFbo();

private:
    QOpenGLContext* m_context;
    QOffscreenSurface* m_offscreenSurface;
    QOpenGLFramebufferObject* m_fbo;
    QTimer m_fullRenderTimer;
    QTimer m_renderTimer;
};
