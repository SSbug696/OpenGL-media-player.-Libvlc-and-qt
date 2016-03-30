#ifndef VIDEOFRAME_H
#define VIDEOFRAME_H

#include <QWidget>

class VideoWindow : public QWidget
{
    Q_OBJECT
public:
    explicit VideoWindow(QWidget *parent = 0);
    QWidget * videoWindow;
signals:

public slots:
};

#endif // VIDEOFRAME_H
