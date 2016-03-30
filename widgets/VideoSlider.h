#ifndef VIDEOSLIDER_H
#define VIDEOSLIDER_H

#include <QWidget>

class QSlider;

class VideoSlider : public QWidget
{
    Q_OBJECT
public:
    explicit VideoSlider(QWidget *parent = 0);
    QSlider * sliderVideo;

signals:

public slots:
};

#endif // VIDEOSLIDER_H
