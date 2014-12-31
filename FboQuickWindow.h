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
    //takes ownership over passed QOpenGLContext
    FboQuickWindow( QOpenGLContext* = 0 );
    ~FboQuickWindow();

    QOpenGLFramebufferObject* fbo() const
        { return m_fbo; }

    //takes ownership over passed QOpenGLContext
    void init( QOpenGLContext* );

    void scheduleResize( const QSize& newSize );
    void scheduleResize( int w, int h );

    void resize( const QSize& newSize );
    void resize( int w, int h );

private:
    using QQuickWindow::resize; //it shouldn't be used directly

protected:
    void resizeEvent( QResizeEvent* );

private Q_SLOTS:
    void sceneGraphInitialized();
    void sceneGraphInvalidated();

    void renderRequested();
    void sceneChanged();

    void doScheduledResize();

    void render();

private:
    void createFbo();
    void destroyFbo();

private:
    QOpenGLContext* m_context;
    QOffscreenSurface* m_offscreenSurface;
    QOpenGLFramebufferObject* m_fbo;

    QTimer m_resizeTimer;
    QSize m_requestedSize;

    bool m_needPolishAndSync;
    QTimer m_renderTimer;
};
