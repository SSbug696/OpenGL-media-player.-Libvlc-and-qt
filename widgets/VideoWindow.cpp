#include "VideoWindow.h"
#if QT_VERSION >= 0x050000
    #include <QtWidgets/QApplication>
    #include <QtWidgets/QDesktopWidget>
    #include <QtWidgets/QHBoxLayout>
    #include <QtWidgets/QToolBar>
#else
    #include <QtGui/QApplication>
    #include <QtGui/QDesktopWidget>
    #include <QtGui/QHBoxLayout>
    #include <QtGui/QToolBar>
#endif

#if defined(Q_WS_X11)
    #include <X11/Xlib.h>
    #include <qx11info_x11.h>
#endif
#include <QPushButton>
#include <QLabel>
#include <QGridLayout>
#include <QPainter>
#include <QGraphicsOpacityEffect>
#include <QBitmap>
#include <QDialog>
#include <QFrame>
#include <QLayout>


VideoWindow::VideoWindow(QWidget *parent) : QWidget(parent)
{

    QHBoxLayout *layout = new QHBoxLayout;
    videoWindow = new QWidget(parent);
    videoWindow->setGeometry(0, 0, 800, 490);
    videoWindow->setBaseSize(QSize(800, 490));
    videoWindow->setMouseTracking(true);
    videoWindow->setStyleSheet(QString("background-color: #000;width:100%;height:100%;"));
    videoWindow->setAutoFillBackground(true);
   // videoWindow-> //setStretchFactor(0, 100);
    //videoWindow->show();
    layout->addWidget(videoWindow);
    layout->setStretch(1, 0);
    parent->setLayout(layout);




#ifndef Q_WS_X11
   videoWindow->setAttribute( Qt::WA_PaintOnScreen, false );
#endif


}

