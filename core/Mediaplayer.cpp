#include "Mediaplayer.h"
#include <vlc/vlc.h>
#include <QtCore/QDateTime>
#include <QCommandLineOption>
#include <QCommandLineParser>

#include "Video.h"
#include "Audio.h"
#include "Media.h"
#include "VideoShow.h"
#include "Instance.h"
#include "iostream"
#include <fstream>
#include <unistd.h>
#include <string>
#include "stdio.h"
#include <strstream>
#include <sstream>
#include <thread>
#include <memory>

#include <QMutex>
#include <QtCore>
#include <QtTest/qtest.h>
#include <vlc/libvlc_structures.h>
#include <vlc/libvlc.h>
#include <time.h>


struct libvlc_event_t;
struct libvlc_event_manager_t;
struct libvlc_media_t;
struct libvlc_media_player_t;
struct libvlc_instance_t;
struct libvlc_new;
struct libvlc_module_description_t; //* libvlc_video_filter_list_get

const int WIDTH  = 5000;
const int HEIGHT = 5000;
unsigned char * data_pixels = new unsigned char[ WIDTH *  HEIGHT * 3];

bool status=false;

/*!
 * \brief MediaPlayer::MediaPlayer
 * \param instance
 * \param ui
 */
MediaPlayer::MediaPlayer(Instance * instance){
    _mp =  libvlc_media_player_new(instance->getInstance());
    _instance = instance;
    _mpEvents = libvlc_media_player_event_manager(_mp);

    _mpVideo =  std::unique_ptr<Video> (new Video(_mp));
    _mpAudio  = std::unique_ptr<Audio> (new Audio(_mp));

    state=0;
    pix   = data_pixels;
}


void MediaPlayer::release(){
    libvlc_media_player_stop(_mp);
    libvlc_media_player_release(_mp);
}


MediaPlayer::~MediaPlayer(){   
    release();
}


/*!
 * \brief MediaPlayer::voice
 * \return
 */
void MediaPlayer::voice(int val){
    _mpAudio->setVolume(val);
}


/*!
 * \brief MediaPlayer::mute
 * \return
 */
void MediaPlayer::mute(){
    _mpAudio->toogleMute();
}

/*!
 * \brief MediaPlayer::setPosition
 * \param t
 */
void MediaPlayer::setPosition(float t){
    libvlc_media_player_navigate(_mp, t);
    libvlc_media_player_set_position(_mp, t);
    libvlc_media_player_set_time(_mp, t);
    //libvlc_toggle_fullscreen(_mp);
}

/*!
 * \brief MediaPlayer::getCurentMedia
 * \return
 */
Media * MediaPlayer::getCurentMedia() const{
    return _mpMedia;
}

/*!
 * \brief MediaPlayer::length
 * \return
 */
int MediaPlayer::length() const {
    libvlc_time_t length = libvlc_media_player_get_length(_mp);
    return length;
}

/*!
 * \brief MediaPlayer::getTime
 * \return
 */
int MediaPlayer::getTime() const {
    libvlc_time_t time = libvlc_media_player_get_time(_mp);
    return time;
}
/*!
 * \brief MediaPlayer::getInstance
 * \return
 */
libvlc_media_player_t * MediaPlayer::getInstance(){
    return _mp;
}


unsigned char * MediaPlayer::getPixels(){
    return pix;
}


void print(std::string::size_type n, std::string const &s){
    if (n == std::string::npos) {
        std::cout << "not found\n";
    } else {
        std::cout << "found: " << s.substr(n) << '\n';
    }
}


std::string ssystem (const char *command){
    char tmpname [L_tmpnam];
    std::tmpnam ( tmpname );
    std::string scommand = command;
    std::string cmd = scommand + " >> " + tmpname;
    std::system(cmd.c_str());
    std::ifstream file(tmpname, std::ios::in );
    std::string result;
        if (file) {
      while (!file.eof()) result.push_back(file.get());
          file.close();
    }
    remove(tmpname);
    return result;
}


void * lock( void *data, void **p_pixels )
{
    *p_pixels = data_pixels;
    return NULL;
}


void display( void *data, void *id )
{
  (void) data;
}


void unlock( void *data, void *id, void *const *ipixels )
{}


/*
    int set_cb(void **data, char *format, unsigned *rate, unsigned *channels){
       //cx.mutex->lock();
    }
*/

void task1(libvlc_media_player_t *_mp , libvlc_media_t * media, int w, int h){
    libvlc_media_player_set_media(_mp, media);
//UYVY YUYV
    libvlc_video_set_format( _mp, "YUYV", w, h, w/2 * 4);
    libvlc_video_set_callbacks(_mp, lock, unlock, NULL, NULL);

    libvlc_audio_set_format(_mp, "S16N", NULL, NULL);
    //libvlc_audio_set_format_callbacks( _mp, (libvlc_audio_setup_cb)set_cb, NULL);

    libvlc_media_player_play(_mp);
}


/*!
 * \brief MediaPlayer::open
 * \param media
 * \param instance
 * \param wID
 */
void MediaPlayer::open(Media * media, int wID, QString & path, VideoShow * vshow, bool state){
    _wID = wID;
    _mpMedia = media;
    pathToResource = path;

    width_view = 800;
    height_view = 370;

    char tmpname[400];
    //regexp for resolution
    QRegExp rx("(\\d{2,4})");
    //to capture the full path
    QRegExp getDir("(.*\..*/\).*\..*$");
    int pos_n=0, pos = 0;

    QStringList size_list, file_path;
    QString clone_path = path;
    QString current_path, string_search;

    file_path= path.split("/", QString::KeepEmptyParts);
    QString file = file_path.last();

    //get full path
    while ((pos_n = getDir.indexIn(clone_path , pos_n)) != -1) {
        current_path = getDir.cap(1);
        pos_n += getDir.matchedLength();
    }

    std::string line;
    std::string go_to = "cd ";
    //addition full path string in bash command
    go_to += "\""+current_path.toLocal8Bit().toStdString()+"\"";

    std::string bash = " && ffprobe -of flat=s=_  -show_entries stream=height,width ";
    bash = go_to + bash;

    bash += "\""+ file.toLocal8Bit().toStdString() +"\"";
    std::string respone_console = ssystem(bash.c_str());
    std::istringstream iss(respone_console);

    //get request result
    while ( std::getline(iss, line) ) {
       for(int i=0;i<line.length();i++){
           string_search.push_back(line[i]);
       }
    }

    //parse result
    while ((pos = rx.indexIn(string_search , pos)) != -1) {
        size_list << rx.cap(1);
        pos += rx.matchedLength();
    }

    //get size for width and heght
    width_view  = size_list.at(0).toInt();
    height_view = size_list.at(1).toInt();

    //task1(_mp,media->getMedia(), width_view, height_view);
    /*std::thread t1(task1, _mp,media->getMedia(), width_view, height_view);
    t1.join();
    */

    task1(_mp, media->getMedia(), width_view, height_view);
    vshow->run(width_view, height_view);

    qDebug() << width_view << "-- : " << height_view;
    state=1;
}


int MediaPlayer::getHeight(){
    return height_view;
}

int MediaPlayer::getWidth(){
    return width_view;
}

/*!
 * \brief MediaPlayer::seek
 */
void MediaPlayer::seek(){
/*
    libvlc_media_release(_vlcMedia);
    libvlc_media_player_release(_vlcMediaPlayer);
*/
// libvlc_media_player_set_position(_vlcMediaPlayer, 120.0);
}

/*!
 * \brief MediaPlayer::pause
 */
void MediaPlayer::pause(){

    libvlc_media_player_pause(_mp);
    libvlc_media_player_set_pause(_mp, 1);

}

void MediaPlayer::error(){}
