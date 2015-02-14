#pragma once

#include <QQuickWindow>
#include <QQuickRenderControl>
#include <QTimer>

QT_FORWARD_DECLARE_CLASS( QOffscreenSurface )

class FboQuickWindow : public QQuickWindow
{
    Q_OBJECT

public:
    //takes ownership over passed QOpenGLContext
    FboQuickWindow( QOpenGLContext* = 0 );
    ~FboQuickWindow();

    QOpenGLFramebufferObject* fbo() const
        { return m_fbo; }

    //takes ownership over passed QOpenGLContext
    void init( QOpenGLContext* );

    QOpenGLContext* glContext()
        { return m_context; }

    void scheduleResize( const QSize& newSize );
    void scheduleResize( int w, int h );

    void resize( const QSize& newSize );
    void resize( int w, int h );

Q_SIGNALS:
    void sceneRendered();

private:
    using QQuickWindow::resize; //it shouldn't be used directly

protected:
    void resizeEvent( QResizeEvent* );

private Q_SLOTS:
    void sceneGraphInitialized();
    void sceneGraphInvalidated();

    void renderRequested();
    void sceneChanged();

    void render();

private:
    QQuickRenderControl* createRenderControl();

    void createFbo();
    void destroyFbo();

private:
    QQuickRenderControl* m_renderControl;

    QOpenGLContext* m_context;
    QOffscreenSurface* m_offscreenSurface;
    QOpenGLFramebufferObject* m_fbo;

    bool m_needPolishAndSync;
    QTimer m_renderTimer;
};
