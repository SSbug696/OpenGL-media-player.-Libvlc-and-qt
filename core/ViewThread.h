#ifndef VIEWTHREAD_H
#define VIEWTHREAD_H

#include <QObject>
#include <QGLWidget>
#include <QTimer>
#include <QTimerEvent>
#include <QGLContext>
#include <QPointer>


class VideoShow;
class Player;

class ViewThread : public QObject
{
    Q_OBJECT
public:
    QPointer<QGLContext> glContext;
    VideoShow * videoShow;
    QTimer * timer;

    ViewThread(VideoShow *video){
        videoShow = video;
    }
/*
    void timerEvent(QTimerEvent * ev) {
       if (ev->timerId() == timer.timerId()) nextFrame();
     }
*/
    void run();

signals:
public slots:
    void nextFrame();
    void activeAnaglyph();
};

#endif // VIEWTHREAD_H

