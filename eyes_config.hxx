/* Eyes
 * Copyright (C) 2011  Krzysztof Mędrzycki, Damian Chiliński
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
    const key_info    * inf;
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
