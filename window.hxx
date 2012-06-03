/* Eyes
 * Copyright (C) 2011, 2012  Krzysztof Mędrzycki, Damian Chiliński
 * 
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef EYES_WINDOW_HXX
#define EYES_WINDOW_HXX

#include <QWidget>
#include <QSystemTrayIcon>
#include <QStringList>
#include <QMenu>
#include <libconfig.h++>
#include "eyes.hxx"
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
    void  onQuit          ();
    void  mousePressEvent ( QMouseEvent * );
private:
    bool                isicon;
    eyes_view         * eyes;
    QSystemTrayIcon   * trayico;
    QMenu             * timenu;
    QAction           * quitA,
                      * setA;
    Configurator      * config;
};

#endif // EYES_WINDOW_HXX
