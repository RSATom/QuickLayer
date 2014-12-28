#include "FboQuickView.h"

#include <QScopedPointer>
#include <QQuickItem>

FboQuickView::FboQuickView( QOpenGLContext* context /*= 0*/ )
    : FboQuickWindow( context ), m_qmlComponent( 0 ), m_rootItem( 0 )
{
}

FboQuickView::~FboQuickView()
{
}

void FboQuickView::setSource( const QUrl& source )
{
    if( m_rootItem ) {
        delete m_rootItem;
        m_rootItem = 0;
    }

    if( m_qmlComponent ) {
        delete m_qmlComponent;
        m_qmlComponent = 0;
    }

    if( !source.isEmpty() ) {
        m_qmlComponent = new QQmlComponent( &m_qmlEngine, source );
        if( m_qmlComponent->isLoading() ) {
            connect( m_qmlComponent, &QQmlComponent::statusChanged,
                     this, &FboQuickView::componentStatusChanged );
        } else {
            componentStatusChanged( m_qmlComponent->status() );
        }

        QResizeEvent event( size(), size() );
        resizeEvent( &event );
    }
}

void FboQuickView::componentStatusChanged( QQmlComponent::Status status )
{
    Q_ASSERT( !m_rootItem );

    if( QQmlComponent::Ready != status )
        return;

    QScopedPointer<QObject> rootObject( m_qmlComponent->create() );
    m_rootItem = qobject_cast<QQuickItem*>( rootObject.data() );

    if( !m_rootItem )
        return;

    m_rootItem->setParentItem( contentItem() );

    rootObject.take();
}

void FboQuickView::resizeEvent( QResizeEvent* event )
{
    FboQuickWindow::resizeEvent( event );

    if( m_rootItem )
        m_rootItem->setSize( QSizeF( width(), height() ) );
}
