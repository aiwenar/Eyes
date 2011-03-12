#ifndef EYES_WINDOW_HXX
#define EYES_WINDOW_HXX

#include <QWidget>
#include <QSystemTrayIcon>
#include <QStringList>
#include <QMenu>
#include <libconfig.h++>
#include "eyes_view.h"

using namespace libconfig;

class eyes_window : public QWidget
{
    Q_OBJECT
public:
    explicit eyes_window ( QString color, QWidget * parent = 0 );
signals:
public slots:
private:
    bool                isicon;
    eyes_view         * eyes;
    QSystemTrayIcon   * trayico;
    QMenu             * timenu;
    QIcon             * tico;
    QAction           * quitA,
                      * setA;
    Config            * cfg;
};

#endif // EYES_WINDOW_HXX
