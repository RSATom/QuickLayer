#pragma once

#include <QQmlEngine>
#include <QQmlComponent>

#include "FboQuickWindow.h"

class FboQuickView : public FboQuickWindow
{
    Q_OBJECT

public:
    //takes ownership over passed QOpenGLContext
    FboQuickView( QOpenGLContext* = 0 );
    ~FboQuickView();

    QQmlContext* rootContext() const;

    QList<QQmlError> errors() const;

    typedef QQmlComponent::Status Status;
    Status status() const;

Q_SIGNALS:
   void statusChanged( Status );

public Q_SLOTS:
    void setSource( const QUrl& );
    void setQml( const QString& qml, const QUrl& qmlUrl );

private Q_SLOTS:
    void componentStatusChanged( QQmlComponent::Status );

protected:
    void resizeEvent( QResizeEvent* event );

private:
    QQmlEngine m_qmlEngine;
    QQmlComponent* m_qmlComponent;
    QQuickItem* m_rootItem;
};
