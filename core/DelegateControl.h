#ifndef DELEGATECONTROL_H
#define DELEGATECONTROL_H

#include <QObject>

struct libvlc_event_t;
struct libvlc_event_manager_t;
struct libvlc_media_t;
struct libvlc_media_player_t;
struct Mediaplayer;


class DelegateControl : public QObject {
    Q_OBJECT
public:
   // explicit DelegateControl(QObject *parent = 0);
    DelegateControl(Mediaplayer *);

    void play();
    void pause();
    Mediaplayer * _mp;

signals:

public slots:
};

#endif // DELEGATECONTROL_H
