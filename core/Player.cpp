
#include "iostream"
#include "QtCore/QTime"
#include "Filters.h"
#include "Player.h"
#include "ui_player.h"
#include "vlc/libvlc.h"
#include <vlc/vlc.h>

using namespace std;

Player::Player(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Player)
{
#ifdef Q_WS_X11
    if(QX11Info::isCompositingManagerRunning())
        setAttribute(Qt::WA_TranslucentBackground);
#endif
    ui->setupUi(this);
    widget =  parent;

    filters = new Filters();
    //
    //filters->setFilter(Filters::P_Q);
    //filters->setFilter(Filters::P_NO_VIDEO_TITLE_SHOW);
    //filters->setFilter(Filters::P_QUIET);
    //filters->setFilter(Filters::P_IGNORE_CONFIG);
    filters->setFilter(Filters::P_QUIET);
    //filters->setFilter(Filters::P_XLIB);
    //filters->setFilter(Filters::P_IGNORE_CONFIG);
    filters->setFilter(Filters::P_NO_LOOP);
    filters->setFilter(Filters::P_DROP_LATE_FRAME);
    filters->setFilter(Filters::P_NO_OSD);
    filters->setFilter(Filters::P_VOUT);
    filters->setFilter(Filters::P_VMEM);

    instance = new Instance(filters->getArgv());

    // Output all modules of current VLClib
    //instance->showActiveModules();

    mediaPlayer = new MediaPlayer(instance);
    interface = new Interface(this);
}


Player::~Player()
{
    delete mediaPlayer;
    delete interface;
    delete video;
    delete ui;
}


void Player::updateInstance(){}


void Player::resizeEvent(QResizeEvent * resizeEvent)
{
    emit emitResizeEvent(resizeEvent);
}

