#ifndef MEDIAPLAYER_H
#define MEDIAPLAYER_H

#include <QObject>
#include <QtCore/QObject>
#include <QtCore/QString>
#include <QtCore/QUrl>
#include <QtWidgets>
#include "ui_player.h"
#include "Player.h"
#include <QMutex>
#include <memory>

class Instance;
class Media;
class Video;
class Audio;
class VideoShow;

struct libvlc_event_t;
struct libvlc_event_manager_t;
struct libvlc_media_t;
struct libvlc_media_player_t;
struct libvlc_media_list_player_t;


/*!
 * \brief The MediaPlayer class
 */
class MediaPlayer: public QObject {
    Q_OBJECT
public:
    MediaPlayer(Instance *);
    ~MediaPlayer();

    static unsigned char * pixels;

    Media * getCurentMedia() const;
    libvlc_media_player_t * getInstance();


    int getTime() const;
    int length() const;
    void open(Media *, int, QString &, VideoShow *, bool);
    void setPosition(float);
    void error();

    libvlc_media_player_t 	* _mp;
    std::unique_ptr<Video> _mpVideo;
    std::unique_ptr<Audio> _mpAudio;
    Media * _mpMedia;
    QString pathToResource;
    Instance * _instance;

    int getHeight();
    int getWidth();

    unsigned char * getPixels();

    int width_view,
        height_view;
    unsigned char * pix;
    int state;


    void voice(int);
    void mute();

public slots:
    void pause();
    void seek();
    void release();

private slots:


private:

    libvlc_event_manager_t *_mpEvents;
    Ui::Player * _ui;
    int currentTime;
    int _wID;
    //WId _focusWID;

};

#endif // MEDIAPLAYER_H
