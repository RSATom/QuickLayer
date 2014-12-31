#include "FboQuickWindow.h"

#include <QOpenGLContext>
#include <QOffscreenSurface>
#include <QOpenGLFramebufferObject>
#include <QQuickRenderControl>

#include <QTimer>

FboQuickWindow::FboQuickWindow( QOpenGLContext* context /*= 0*/ )
    : QQuickWindow( &m_renderControl ),
      m_context( 0 ), m_offscreenSurface( 0 ), m_fbo( 0 )
{
    connect( this, &QQuickWindow::sceneGraphInitialized,
             this, &FboQuickWindow::sceneGraphInitialized );
    connect( this, &QQuickWindow::sceneGraphInvalidated,
             this, &FboQuickWindow::sceneGraphInitialized );

    connect( &m_renderControl, &QQuickRenderControl::renderRequested,
             this, &FboQuickWindow::renderRequested );
    connect( &m_renderControl, &QQuickRenderControl::sceneChanged,
             this, &FboQuickWindow::sceneChanged );

    m_fullRenderTimer.setSingleShot( true );
    m_fullRenderTimer.setInterval( 5 );
    connect( &m_fullRenderTimer, &QTimer::timeout,
             this, &FboQuickWindow::fullRender );

    m_renderTimer.setSingleShot( true );
    m_renderTimer.setInterval( 5 );
    connect( &m_renderTimer, &QTimer::timeout,
             this, &FboQuickWindow::render );

    if( context )
        init( context );
}

FboQuickWindow::~FboQuickWindow()
{
    disconnect( this, &QQuickWindow::sceneGraphInitialized,
                this, &FboQuickWindow::sceneGraphInitialized );
    disconnect( this, &QQuickWindow::sceneGraphInvalidated,
                this, &FboQuickWindow::sceneGraphInitialized );

    disconnect( &m_renderControl, &QQuickRenderControl::renderRequested,
                this, &FboQuickWindow::renderRequested );
    disconnect( &m_renderControl, &QQuickRenderControl::sceneChanged,
                this, &FboQuickWindow::sceneChanged );

    if( m_context && m_offscreenSurface &&
        m_context->makeCurrent( m_offscreenSurface ) )
    {
        m_renderControl.invalidate();
    }

    destroyFbo();

    delete m_offscreenSurface;
}

void FboQuickWindow::init( QOpenGLContext* extContext )
{
    Q_ASSERT( extContext );
    Q_ASSERT( !m_context && !m_offscreenSurface && !m_fbo );

    m_context = extContext;
    m_context->setParent( this );

    m_offscreenSurface = new QOffscreenSurface();
    m_offscreenSurface->setFormat( m_context->format() );
    m_offscreenSurface->create();

    m_context->makeCurrent( m_offscreenSurface );
    m_renderControl.initialize( m_context );
    m_context->doneCurrent();
}

void FboQuickWindow::createFbo()
{
    Q_ASSERT( !m_fbo );

    QSize size = this->size();
    if( m_context && size.width() && size.height() ) {
        m_fbo =
            new QOpenGLFramebufferObject( size,
                                          QOpenGLFramebufferObject::CombinedDepthStencil );
        setRenderTarget( m_fbo );
    }
}

void FboQuickWindow::destroyFbo()
{
    delete m_fbo;
    m_fbo = 0;
}

void FboQuickWindow::sceneGraphInitialized()
{
    createFbo();
}

void FboQuickWindow::sceneGraphInvalidated()
{
    destroyFbo();
}

void FboQuickWindow::renderRequested()
{
    if( !m_renderTimer.isActive() )
        m_renderTimer.start();
}

void FboQuickWindow::sceneChanged()
{
    if( !m_fullRenderTimer.isActive() )
        m_fullRenderTimer.start();
}

void FboQuickWindow::render()
{
    //FIXME! better do it in separate thread
    if( m_context->makeCurrent( m_offscreenSurface ) ) {
        m_renderControl.render();

        resetOpenGLState();

        m_context->doneCurrent();
    }
}

void FboQuickWindow::fullRender()
{
    //FIXME! better do it in separate thread
    if( m_context->makeCurrent( m_offscreenSurface ) ) {
        m_renderControl.polishItems();
        m_renderControl.sync();
        m_renderControl.render();

        resetOpenGLState();

        m_context->doneCurrent();
    }
}

void FboQuickWindow::resizeEvent( QResizeEvent* )
{
    if( m_context && m_context->makeCurrent( m_offscreenSurface ) ) {
        destroyFbo();
        createFbo();
        m_context->doneCurrent();
    }
}

void FboQuickWindow::resize( const QSize& newSize )
{
    //have to emulate resize event since Qt don't generate some events on hidden window
    QResizeEvent event( size(), newSize );
    QQuickWindow::resize( newSize );
    resizeEvent( &event );
}

void FboQuickWindow::resize( int w, int h )
{
    resize( QSize( w, h ) );
}
