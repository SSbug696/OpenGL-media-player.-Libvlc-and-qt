#include "Media.h"
#include <QtCore/QDir>
#include <vlc/libvlc.h>
#include <vlc/libvlc_media.h>
#include <vlc/libvlc_events.h>
#include <vlc/libvlc_media_list.h>

/*
    #include <vlc/libvlc_media_list_player.h>
    #include <vlc/libvlc_media_library.h>
    #include <vlc/libvlc_media.h>
*/

#include "Instance.h"

/*!
 * \brief Media::Media
 * \param location
 * \param locFileFlag
 * \param instance
 */
Media::Media(Instance * instance, QString path){
  Init(instance, path) ;
}

/*!
 * \brief Media::Init
 * \param location
 * \param localFile
 * \param instance
 */
void Media::Init(Instance *inst, QString & path)
{
    _instance = inst->getInstance();
    _locationSource = path;
    _media = libvlc_media_new_path(inst->getInstance(), path.toUtf8().data());

}

void Media::setMediaToPlaylist(QString path){
  /*  path = QDir::toNativeSeparators(path);
    libvlc_media_t * ptr_media = libvlc_media_new_path(_instance, path.toUtf8().data());
    libvlc_media_add_media(_media_list, ptr_media);
  */
}


/*!
 * \brief Media::getMedia
 * \return
 */
libvlc_media_t * Media::getMedia() const {
    return _media;
}

int Media::getMediaListCount() const {
    //return libvlc_media_count(_media_list);
}

/*!
 * \brief Media::getEventManager
 * \return
 */
libvlc_event_manager_t * Media::getEventManager() const {
    return _events;
}

/*!
 * \brief Media::~Media
 */
Media::~Media(){
    libvlc_media_release(_media);
}
