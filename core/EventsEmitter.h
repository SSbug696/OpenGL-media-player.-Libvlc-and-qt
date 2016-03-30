#ifndef EVENTSEMITTER_H
#define EVENTSEMITTER_H

#include <QObject>
#include <vlc/libvlc.h>
#include <vlc/vlc.h>
#include <vlc/libvlc_events.h>

struct libvlc_event_t;
class  Interface;

class EventsEmitter : public QObject
{
    Q_OBJECT

private:
    Interface * _interface;


public:
    explicit EventsEmitter(QObject *parent = 0);
    EventsEmitter(Interface *);

   // void callbackEvents(const libvlc_event_t *, void *);
    libvlc_event_manager_t * events;
    void bindEvents();
    void unbindEvents();
    void Playing();
    void PositionChanged();
    void TimeChanged();
    void Paused();
    void Stopped();
    void Opening();
    void Buffering();
    void Vout();

signals:
public slots:
};

#endif // EVENTSEMITTER_H
