#pragma once

#include <QWindow>
#include <QOpenGLFunctions>

QT_FORWARD_DECLARE_CLASS( QOpenGLShaderProgram )

class FboQuickWindow; //#include "FboQuickWindow.h"

class FboQuickWrapperWindow
    : public QWindow,
      protected QOpenGLFunctions
{
    Q_OBJECT

public:
    FboQuickWrapperWindow( FboQuickWindow*, QWindow* parent = nullptr );

protected:
    void exposeEvent( QExposeEvent* ) override;
    void resizeEvent( QResizeEvent* ) override;

    void keyPressEvent( QKeyEvent* ) override;
    void keyReleaseEvent( QKeyEvent* ) override;
    void mouseMoveEvent( QMouseEvent* ) override;
    void mousePressEvent( QMouseEvent* ) override;
    void mouseReleaseEvent( QMouseEvent* ) override;
    void wheelEvent( QWheelEvent* ) override;

private:
    void render();

private:
    FboQuickWindow* m_fboWindow;
    QOpenGLContext* m_context;

    QOpenGLShaderProgram* m_program;
};
