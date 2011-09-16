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

#ifndef _eyes_configurator_hxx
#define _eyes_configurator_hxx

#include <QWidget>
#include <QDialog>
#include <QTabWidget>
#include <QGroupBox>
#include <QComboBox>
#include <QSpinBox>
#include <QLineEdit>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>

class Configurator : public QDialog
{
    Q_OBJECT
public:
    explicit    Configurator    ( QWidget * parent = 0 );
    void        set_icon        ( QIcon * icn );

    static  void    set_main    ();
public slots:
    void    on_accept   ();
    void    on_cancel   ();
private:
    QIcon         * icon;
    QTabWidget    * tabs;
    QVBoxLayout   * ml;
    QHBoxLayout   * bl;
    QPushButton   * accept,
                  * cancel;
};

class ConfTabUi : public QWidget
{
    Q_OBJECT
public:
    explicit    ConfTabUi   ( QWidget * parent = 0 );
private:
    QGroupBox     * geyes,
                  * glooker,
                  * gclapper;
    QComboBox     * ccolor,
                  * ctheme;
    QSpinBox      * seye,
                  * smirror,
                  * swidth,
                  * sheight,
                  * sldxmax,
                  * sldxmin,
                  * sldymax,
                  * sldymin,
                  * sldmax,
                  * sldmin,
                  * scdmax,
                  * scdmin;
};

#endif //eyes_configurator_hxx
