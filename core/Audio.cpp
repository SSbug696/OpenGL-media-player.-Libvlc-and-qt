#include "vlc/vlc.h"
#include "Audio.h"
#include "Mediaplayer.h"

Audio::Audio(libvlc_media_player_t * player)
{
    mp = player;
}

Audio::~Audio(){
    delete mp;
}

bool Audio::getMute() const {
    bool mute = false;
    if (mp) {
        mute = libvlc_audio_get_mute(mp);
    }
    return mute;
}

void Audio::setVolume(int volume){
    if (mp) {
        if (volume != getVolume()) {
            libvlc_audio_set_volume(mp, volume);
        }
    }
}

bool Audio::toogleMute() const{
    if (mp) {
        libvlc_audio_toggle_mute(mp);
    }
    return getMute();
}

int Audio::getVolume(){
    int volume = -1;
    if (mp) {
        volume = libvlc_audio_get_volume(mp);
    }

    return volume;
}
