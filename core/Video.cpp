#include "Video.h"
#include <vlc/vlc.h>
#include <iostream>
#include <QDebug>

Video::Video(QObject *parent) : QObject(parent)
{}

Video::Video(libvlc_media_player_t * player){
    mp=player;
}

bool Video::getSnapshot(const QString & path){
    bool status = 0;
    if (mp && libvlc_media_player_has_vout(mp)) {
        status = libvlc_video_take_snapshot(mp, 0, path.toUtf8().data(), 0, 0) + 1;
    }
    return status;
}


void Video::showLogo(const QString & path, int x, int y, int opacity){
    if(mp && libvlc_media_player_has_vout(mp)){
        libvlc_video_set_logo_string((mp), libvlc_logo_file, path.toUtf8().data());
        libvlc_video_set_logo_int((mp), libvlc_logo_x, x);
        libvlc_video_set_logo_int((mp), libvlc_logo_y, y);
        libvlc_video_set_logo_int((mp), libvlc_logo_opacity, opacity);
        libvlc_video_set_logo_int((mp), libvlc_logo_enable, 1);
    }
}


void Video::setRatio(QString & param){
    if (mp && libvlc_media_player_has_vout(mp)) {
        libvlc_video_set_aspect_ratio(mp, param.toUtf8().data());
    }
}


void Video::setPause(){
    libvlc_media_player_pause(mp);
    libvlc_media_player_set_pause(mp, 1);
}

int Video::getTrackCount() const{
    int count = -1;
    if (mp){
        count = libvlc_video_get_track_count(mp);
    }
    return count;
}


// Get list of id's
std::vector<int> Video::getTrackId() const{
    std::vector<int> list_id;

    if (mp) {
        libvlc_track_description_t *desc;
        int track_count = getTrackCount();

        desc = libvlc_video_get_track_description(mp);
        list_id.push_back(desc->i_id);

        if (track_count > 1) {
            for(int i = 1; i < track_count; i++) {
                desc = desc->p_next;
                list_id.push_back(desc->i_id);
            }
        }
    }
    return list_id;
}
