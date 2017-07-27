#include "Instance.h"
#include <vlc/libvlc.h>
#include <iostream>
#include <vector>
#include "Filters.h"
#include <QDebug>
#include <QString>


Instance::Instance(std::vector<const char *> & filters){
    const char * argv[filters.size()];
    for(int i=0;i<=filters.size();i++){
        argv[i]=filters[i];
    }
    _instance=libvlc_new(filters.size(), argv);
}


libvlc_instance_t * Instance::getInstance(){
    return _instance;
}


Instance::~Instance(){
    delete _instance;
}


void Instance::showActiveModules(){
    libvlc_module_description_t *t =libvlc_video_filter_list_get(_instance);
    qDebug() << QString::fromStdString( (char *)t->psz_longname );
    qDebug() << QString::fromStdString( (char *)t->p_next->psz_longname );

    while(t){
        qDebug() << QString::fromStdString( (char *)t->psz_shortname );
        t=t->p_next;
     }
}
