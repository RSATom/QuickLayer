#include "FboQuickWrapperWindow.h"

#include <QCoreApplication>
#include <QOpenGLContext>
#include <QOpenGLFramebufferObject>
#include <QOpenGLShaderProgram>

#include "FboQuickWindow.h"

FboQuickWrapperWindow::FboQuickWrapperWindow( FboQuickWindow* fqw,
                                              QWindow* parent /*= nullptr*/ )
    : QWindow( parent ), m_fboWindow( fqw ), m_context( nullptr ),
      m_program( nullptr )
{
    setSurfaceType(QSurface::OpenGLSurface);

    QOpenGLContext* sourceContext = fqw->glContext();
    QSurfaceFormat format = sourceContext->format();

    setFormat( format );

    m_context = new QOpenGLContext( this );
    m_context->setShareContext( sourceContext );
    m_context->setFormat( format );
    m_context->create();

    connect( fqw, &FboQuickWindow::sceneRendered,
             this, &FboQuickWrapperWindow::render );
}

void FboQuickWrapperWindow::resizeEvent( QResizeEvent* e )
{
    m_fboWindow->resize( e->size() );
}

void FboQuickWrapperWindow::exposeEvent( QExposeEvent* )
{
    render();
}

void FboQuickWrapperWindow::render()
{
    if( !isExposed() )
        return;

    m_context->makeCurrent( this );

    enum {
        aVertex = 0,
        aTexCoord = 1,
    };

    if( !m_program ) {
       initializeOpenGLFunctions();

        static const char* vertexShaderSource =
            "attribute highp vec4 a_vertex;"
            "attribute lowp vec2 a_texCoord;"
            "varying lowp vec2 v_texCoord;"
            "void main() {"
            "   v_texCoord = a_texCoord;"
            "   gl_Position = a_vertex;"
            "}";

        static const char* fragmentShaderSource =
            "varying lowp vec2 v_texCoord;"
            "uniform sampler2D tex;"
            "void main() {"
            "   gl_FragColor = vec4( texture2D( tex, v_texCoord ).rgb, 1.0 );"
            "}";

        m_program = new QOpenGLShaderProgram( m_context );
        m_program->addShaderFromSourceCode( QOpenGLShader::Vertex,
                                            vertexShaderSource );
        m_program->addShaderFromSourceCode( QOpenGLShader::Fragment,
                                            fragmentShaderSource );
        m_program->link();

        m_program->bindAttributeLocation( "a_vertex", aVertex );
        m_program->bindAttributeLocation( "a_texCoord", aTexCoord );
    }

    if( QOpenGLFramebufferObject* fbo = m_fboWindow->fbo() ) {
        glViewport(0, 0, width(), height());

        glBindTexture( GL_TEXTURE_2D, fbo->texture() );

        m_program->bind();

        //FIXME! replace with vbo
        static const GLfloat v[] = {
            -1. ,  1. ,  0. ,
             1. , -1. ,  0. ,
            -1. , -1. ,  0. ,

             1. , -1. ,  0. ,
            -1. ,  1. ,  0. ,
             1. ,  1. ,  0. ,
        };

        static const GLfloat t[] = {
            0., 1.,
            1., 0.,
            0., 0.,

            1., 0.,
            0., 1.,
            1., 1.,
        };

        glVertexAttribPointer( aVertex, 3, GL_FLOAT, GL_FALSE, 0, v );
        glEnableVertexAttribArray( 0 );

        glVertexAttribPointer( aTexCoord, 2, GL_FLOAT, GL_FALSE, 0, t );
        glEnableVertexAttribArray( 1 );

        glDrawArrays( GL_TRIANGLES, 0, 6 );

        m_program->release();
    }

    m_context->swapBuffers( this );
}

void FboQuickWrapperWindow::mouseMoveEvent( QMouseEvent* e )
{
    QMouseEvent mouseEvent( e->type(), e->localPos(), e->localPos(),
                            e->button(), e->buttons(), e->modifiers() );
    QCoreApplication::sendEvent( m_fboWindow, &mouseEvent );
}

void FboQuickWrapperWindow::mousePressEvent( QMouseEvent* e )
{
    QMouseEvent mouseEvent( e->type(), e->localPos(), e->localPos(),
                            e->button(), e->buttons(), e->modifiers() );
    QCoreApplication::sendEvent( m_fboWindow, &mouseEvent );
}

void FboQuickWrapperWindow::mouseReleaseEvent( QMouseEvent* e )
{
    QMouseEvent mouseEvent( e->type(), e->localPos(), e->localPos(),
                            e->button(), e->buttons(), e->modifiers() );
    QCoreApplication::sendEvent( m_fboWindow, &mouseEvent );
}

void FboQuickWrapperWindow::keyPressEvent( QKeyEvent* e )
{
    QCoreApplication::sendEvent( m_fboWindow, e );
}

void FboQuickWrapperWindow::keyReleaseEvent( QKeyEvent* e )
{
    QCoreApplication::sendEvent( m_fboWindow, e );
}

void FboQuickWrapperWindow::wheelEvent( QWheelEvent* e )
{
    QWheelEvent wheelEvent( e->pos(), e->pos(), e->delta(),
                            e->buttons(), e->modifiers(), e->orientation() );
}
