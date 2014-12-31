#include "MainWidget.h"

#include "../QuickLayer.h"

MainWidget::MainWidget()
{
    NSView* nsView = reinterpret_cast<NSView*>( this->winId() );

    QuickLayer* nsOglLayer =
        [[QuickLayer alloc] initWithFboQuickWindow: &m_fboQuickView];
    [nsOglLayer setAsynchronous:YES];
    [nsView setLayer: nsOglLayer];
    [nsView setWantsLayer: YES];

    m_fboQuickView.setSource( QUrl( QStringLiteral( "qrc:/demo.qml" ) ) );
}

MainWidget::~MainWidget()
{
}
