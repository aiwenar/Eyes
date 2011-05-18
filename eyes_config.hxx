#ifndef EYES_CONFIG_HXX
#define EYES_CONFIG_HXX

#include <QWidget>
#include <QTabWidget>
#include <QIcon>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QPlainTextEdit>
#include <QVector>
#include <QLineEdit>
#include <QScrollArea>
#include <libconfig.h++>

#include <econfig.hxx>

using namespace libconfig;

struct key_info
{
    char * path;
    char * type;
    char * desc;
};

namespace eyes_cfg_v
{

class cfga_item : public QWidget
{
    Q_OBJECT
public:
    explicit    cfga_item   ( const key_info& what, Config * cfg, QWidget * parent = 0 );
    void        show        ();
private:
    const key_info    * info;
    Setting           * set;
    QHBoxLayout       * hbl;
    QLabel            * path;
    QLineEdit         * tedit;
};

class cfg_advanced : public QWidget
{
    Q_OBJECT
public:
    explicit    cfg_advanced   ( eConfig& ncfg, QWidget * parent = 0 );
    void        set_config     ( eConfig& ncfg );
    void        show           ();
signals:
public slots:
private:
    eConfig            * cfg;
    QVBoxLayout       * list;
    QVector<cfga_item*> items;
};

};

class eyes_config : public QWidget
{
    Q_OBJECT
public:
    explicit    eyes_config ( eConfig& ncfg, QWidget * parent = 0 );
    void        set_icon    ( QString suffix );
signals:
public slots:
private:
    QTabWidget                * tabs;
    QIcon                     * icon;
    eConfig                   * cfg;
    eyes_cfg_v::cfg_advanced  * adv;
};

#endif // EYES_CONFIG_HXX
