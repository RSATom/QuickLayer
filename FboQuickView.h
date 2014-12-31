#pragma once

#include <QQmlEngine>
#include <QQmlComponent>

#include "FboQuickWindow.h"

class FboQuickView : public FboQuickWindow
{
public:
    //takes ownership over passed QOpenGLContext
    FboQuickView( QOpenGLContext* = 0 );
    ~FboQuickView();

    QQmlContext* rootContext() const;

public Q_SLOTS:
    void setSource( const QUrl& );

private Q_SLOTS:
    void componentStatusChanged( QQmlComponent::Status );

protected:
    void resizeEvent( QResizeEvent* event );

private:
    QQmlEngine m_qmlEngine;
    QQmlComponent* m_qmlComponent;
    QQuickItem* m_rootItem;
};
