#include "Instance.h"
#include <vlc/libvlc.h>
#include <iostream>
#include <vector>
#include "Filters.h"


Instance::Instance(std::vector<const char *> & filters)
{
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
    std::cout << (char *)t->psz_longname << std::endl;
    std::cout << (char *)t->p_next->psz_longname << std::endl;
    while(t){
        std::cout << (char *)t->psz_shortname << std::endl;
        t=t->p_next;
     }
}
