#ifndef INSTANCE_H
#define INSTANCE_H

#include <QObject>
#include <QMap>

class Filters;
struct libvlc_module_description_t;
struct libvlc_instance_t;

/*!
 * \brief The Instance class
 */
class Instance: public QObject {
    Q_OBJECT
public:
    Instance(std::vector<const char *> &);

    ~Instance();

    libvlc_instance_t * getInstance();
    void showActiveModules();

private:
    libvlc_instance_t * _instance;
};

#endif // INSTANCE_H
