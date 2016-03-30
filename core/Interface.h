#ifndef INTERFACE_H
#define INTERFACE_H

#include <QObject>
#include <QResizeEvent>
#include <QThread>


class Player;
class VideoSlider;
class VideoWindow;
class ControlPanel;
class EventsEmitter;
class VideoShow;
class ViewThread;
class QString;

struct libvlc_event_t;
struct libvlc_event_manager_t;
struct libvlc_media_t;
struct libvlc_media_player_t;

class Interface: public QObject
{
    Q_OBJECT
public:
    Interface(Player *);
    Player  *ptrP;
    double scaleX;
    double scaleY;
    int windowW;
    int windowH;

    QThread *thread;

    VideoShow * view;
    ViewThread * showMediaThread;

public slots:
    void openLocal();
    void mute();
    void valueChange(int);
    void trackSlider();
    void getLastTime();
    void eventsEmitter(QString);
    void enabled3d();
    //void displayUpdate();
    void updateStatusControlPanel();
    void resizeEvent(QResizeEvent *);
    void changeRatio();

    void play();
    void playEvent();
    void openEvent();
    void playingEvent();
    void stopEvent();
    void bufferingEvent();
    void outFinishedEvent();
    void timeChangeEvent();
    void pause();
    void triggerSliderMove();

private slots:
    void fullscreen();

private:
    EventsEmitter * eEmitter;
    int _winID;
    ControlPanel * _controlPanel;
    VideoSlider * _videoSlider;
    VideoWindow * _videoWindow;
    bool _is_pause_event;
    int _time_stopped;
    QString _source_path;

};

#endif // INTERFACE_H
