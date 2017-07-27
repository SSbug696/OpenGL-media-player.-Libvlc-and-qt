#ifndef AUDIO_H
#define AUDIO_H
#include <QObject>


class MediaPlayer;

struct libvlc_media_player_t;
/*!
 * \brief The Audio class
 */
class Audio {
public:
    Audio(libvlc_media_player_t *);
    ~Audio();
    bool getMute() const;
    bool toogleMute() const;
    void setVolume(int volume);
    int getVolume();

private:
     libvlc_media_player_t * mp;
};

#endif // AUDIO_H
