#pragma once

#include <QWidget>

#include "../FboQuickView.h"

class MainWidget : public QWidget
{
    Q_OBJECT

public:
    MainWidget();
    ~MainWidget();

private:
    FboQuickView m_fboQuickView;
};
