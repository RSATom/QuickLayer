#include <QGuiApplication>
#include <QOpenGLContext>

#include "FboQuickView.h"
#include "FboQuickWrapperWindow.h"

int main( int argc, char *argv[] )
{
    QGuiApplication app( argc, argv );

    QSize size( 640, 480 );

    QOpenGLContext* context = new QOpenGLContext;
    context->create();
    FboQuickView fboQuickView( context );
    fboQuickView.resize( size );
    fboQuickView.setSource( QUrl( QStringLiteral( "qrc:/demo.qml" ) ) );

    FboQuickWrapperWindow window( &fboQuickView );
    window.resize( size );
    window.show();

    return app.exec();
}
