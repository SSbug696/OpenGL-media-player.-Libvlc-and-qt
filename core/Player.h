#ifndef PLAYER_H
#define PLAYER_H

#include <QMainWindow>
#include <QtWidgets>

#include "Instance.h"
#include "Interface.h"
#include "Mediaplayer.h"
#include "Media.h"
#include "Video.h"
#include "Audio.h"

struct libvlc_event_t;
struct libvlc_event_manager_t;
struct libvlc_media_t;
struct libvlc_media_player_t;
struct libvlc_instance_t;
struct libvlc_new;
struct libvlc_event_manager_t;

class Filters;
class Interface;
class Video;
class Audio;
class MediaPlayer;
class Media;
class EventsEmitter;

namespace Ui {
class Player;
}

class Player : public QMainWindow {
    Q_OBJECT

public:
    Ui::Player *ui;
    explicit Player(QWidget *parent = 0);
    ~Player();

    QWidget   * widget;
    Filters   * filters;
    Instance  * instance;
    Interface * interface;
    Video     * video;
    Audio     * audio;
    Media     * media;
    EventsEmitter * eventManager;
    MediaPlayer * mediaPlayer;

    void updateInstance();
    void resizeEvent(QResizeEvent *);
    int wId;

signals:
    void emitResizeEvent(QResizeEvent *);

private slots:


private:

};

#endif // PLAYER_H
