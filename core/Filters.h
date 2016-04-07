#ifndef FILTERS_H
#define FILTERS_H

#endif // FILTERS
#include <QMap>
#include <vector>

class Filters {
public:
    static const int
        P_VMEM  =0,
        P_Q     =1,
        P_QUIET =2,
        P_IGNORE_CONFIG=3,
        P_VOUT  =4,
        P_XLIB  =5,
        P_I     =6,
        P_DUMY  =7,
        P_NO_VIDEO_TITLE_SHOW=8,
        P_V     =9,
        P_INTF_DUMMY=10,
        P_NO_OSD=11,
        P_NO_LOOP=12,
        P_DROP_LATE_FRAME=13,
        P_ANAGLYPH_FILTER=14;

        std::vector<const char *> & getArgv();


    Filters();
    void setFilter(int);
    void removeFilter(int);
    void updateListFilters();
    bool checkEnable(int);
    void toogleFilter(int);


private:
    std::vector<const char *> _filters;
    QMap<QString, int> _filters_collection;
    QMap<int, const char *> _filters_args;
    std::vector<int> _activeFilters;
};




