#include "Filters.h"
#include <iostream>
#include <vector>
#include <QString>
Filters::Filters(){
    _filters_collection[QString("no-filter")] = 0;
    //_filters_collection[QString("anaglyph-filter")]=14;

    _filters_args[0]="vmem";
    _filters_args[1]="-q";
    _filters_args[2]="--quiet";
    _filters_args[3]="--ignore-config";
    _filters_args[4]="--vout";
    _filters_args[5]="--no-xlib";
    _filters_args[6]="-I";
    _filters_args[7]="dumy";
    _filters_args[8]="--no-video-title-show";
    _filters_args[9]="-v";
    _filters_args[10]="--intf=dummy";
    _filters_args[11]="--no-osd";
    _filters_args[12]="--no-loop";
    _filters_args[13]="--drop-late-frames";
    _filters_args[14]="--video-filter=Anaglyph";
}


void Filters::setFilter(int id){
     _filters.clear();

     if(std::find(_activeFilters.begin(), _activeFilters.end(), id) == _activeFilters.end()){
        _activeFilters.insert(_activeFilters.end(), id);
     }

     for(unsigned it=0; it!=_activeFilters.size(); it++){
         _filters.insert(_filters.end(), _filters_args[_activeFilters[it]]);
     }

     //STUB
     std::cout << "Current filters list ";
     for(int i=0;i!=_filters.size();i++){
         std::cout << _filters[i] << "  ";
     }
     std::cout << "\n";
}


void Filters::updateListFilters(){
    setFilter(0);
}


bool Filters::checkEnable(int id){
    if(std::find(_activeFilters.begin(), _activeFilters.end(), id) == _activeFilters.end()){
        return false;
    } else return true;
}


void Filters::toogleFilter(int id){
    if(std::find(_activeFilters.begin(), _activeFilters.end(), id) == _activeFilters.end()){
        setFilter(id);
    } else removeFilter(id);
}


void Filters::removeFilter(int id ){
    if(std::find(_activeFilters.begin(), _activeFilters.end(), id) != _activeFilters.end()){
        _activeFilters.erase(_activeFilters.begin()+1);
        updateListFilters();
    }
}


std::vector<const char *> & Filters::getArgv(){
    return _filters;
}
