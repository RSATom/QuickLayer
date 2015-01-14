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

QQmlContext* FboQuickView::rootContext() const
{
    return m_qmlEngine.rootContext();
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
        m_qmlComponent = new QQmlComponent( &m_qmlEngine, &m_qmlEngine );
        connect( m_qmlComponent, &QQmlComponent::statusChanged,
                 this, &FboQuickView::componentStatusChanged );
        m_qmlComponent->loadUrl( source );
    }
}

void FboQuickView::setQml( const QString& qml, const QUrl& qmlUrl )
{
    if( m_rootItem ) {
        delete m_rootItem;
        m_rootItem = 0;
    }

    if( m_qmlComponent ) {
        delete m_qmlComponent;
        m_qmlComponent = 0;
    }

    if( !qml.isEmpty() ) {
        m_qmlComponent = new QQmlComponent( &m_qmlEngine, &m_qmlEngine );
        connect( m_qmlComponent, &QQmlComponent::statusChanged,
                 this, &FboQuickView::componentStatusChanged );
        m_qmlComponent->setData( qml.toUtf8(), qmlUrl );
    }
}

void FboQuickView::componentStatusChanged( QQmlComponent::Status status )
{
    Q_ASSERT( !m_rootItem );

    if( QQmlComponent::Ready != status ) {
        Q_EMIT statusChanged( status );
        return;
    }

    QScopedPointer<QObject> rootObject( m_qmlComponent->create() );
    m_rootItem = qobject_cast<QQuickItem*>( rootObject.data() );

    if( !m_rootItem ) {
        Q_EMIT statusChanged( QQmlComponent::Error );
        return;
    }

    contentItem()->setFocus( true );
    rootObject.take()->setParent( contentItem() );
    m_rootItem->setParentItem( contentItem() );

    QResizeEvent event( size(), size() );
    resizeEvent( &event );

    Q_EMIT statusChanged( QQmlComponent::Ready );
}

void FboQuickView::resizeEvent( QResizeEvent* event )
{
    FboQuickWindow::resizeEvent( event );

    if( m_rootItem )
        m_rootItem->setSize( QSizeF( width(), height() ) );
}

QList<QQmlError> FboQuickView::errors() const
{
    if( m_qmlComponent )
        return m_qmlComponent->errors();

    return QList<QQmlError>();
}

FboQuickView::Status FboQuickView::status() const
{
    if( !m_qmlComponent )
        return QQmlComponent::Null;

    return m_qmlComponent->status();
}
