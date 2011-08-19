#ifndef EYES_WINDOW_HXX
#define EYES_WINDOW_HXX

#include <QWidget>
#include <QSystemTrayIcon>
#include <QStringList>
#include <QMenu>
#include <libconfig.h++>
#include "eyes_view.h"
#include "eyes_config.hxx"
#include <configurator.hxx>

using namespace libconfig;

/**
 * Main window.
 */
class eyes_window : public QWidget
{
    Q_OBJECT
public:
    /**
     * Construct window and initialize subsystems with eyes color \p color.
     * @param color color of the eyes.
     */
    explicit eyes_window    ( QString color, QWidget * parent = 0 );

    QIcon             * tico;
signals:
public slots:
private:
    bool                isicon;
    eyes_view         * eyes;
    eyes_config       * ecfg;
    QSystemTrayIcon   * trayico;
    QMenu             * timenu;
    QAction           * quitA,
                      * setA;
    eConfig           * oldcfg;
    Configurator      * config;
};

#endif // EYES_WINDOW_HXX
