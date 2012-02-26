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

#include <QGridLayout>
#include <QLabel>
#include <QApplication>
#include "configurator.hxx"
#include "configuration.hxx"
#include <debug.hxx>

Configurator::Configurator ( QWidget * parent ) : QDialog ( parent )
{
    info << "(Configurator) setting up configurator...\n";
    resize          ( 647, 400 );
    setWindowTitle  ( "!eyes config" );

    ml = new QVBoxLayout ( this );
    setLayout ( ml );

    bl      = new QHBoxLayout ();
    accept  = new QPushButton ( "accept" );
    cancel  = new QPushButton ( "cancel" );
    bl->addWidget    ( accept );
    bl->addWidget    ( cancel );

    connect ( accept, SIGNAL ( clicked () ), this, SLOT ( on_accept () ) );
    connect ( cancel, SIGNAL ( clicked () ), this, SLOT ( on_cancel () ) );

    tabs = new QTabWidget ();
    tabs->setTabPosition  ( QTabWidget::East );
    tabs->addTab ( new ConfTabUi (), QString ( "eyes" ) );
    ml->addWidget ( tabs );
    ml->addLayout ( bl );
    ml->setAlignment ( bl, Qt::AlignRight );

    info << "(Configurator) ready!\n";
}

void Configurator::set_icon ( QIcon * icn )
{
    icon = icn;
    setWindowIcon ( *icn );
}

bool _conf_is_main = false;

void Configurator::set_main ()
{
    _conf_is_main = true;
}

void Configurator::on_accept ()
{
    hide ();
    if ( _conf_is_main )
        QApplication::quit ();
}

void Configurator::on_cancel ()
{
    hide ();
    if ( _conf_is_main )
        QApplication::quit ();
}

ConfTabUi::ConfTabUi ( QWidget * parent ) : QWidget ( parent )
{
    info << "(Configurator) setting up Ui tab...\n";
    geyes    = new QGroupBox ( "eyes" );
    glooker  = new QGroupBox ( "looking" );
    gclapper = new QGroupBox ( "clapping" );

    Configuration * cfg = Configuration::getInstance ();

    QVBoxLayout * ml = new QVBoxLayout ( this );
    ml->addWidget   ( geyes );
    ml->addWidget   ( glooker );
    ml->addWidget   ( gclapper );
    setLayout       ( ml );

    ccolor  = new QComboBox ();
    ctheme  = new QComboBox ();
    seye    = new QSpinBox  ();
    smirror = new QSpinBox  ();
    swidth  = new QSpinBox  ();
    sheight = new QSpinBox  ();
    ccolor->addItem         ( "green" );
    ctheme->addItem         ( "draw" );
    seye->setValue          ( cfg->lookupValue ( "ui.eye.size", 60 ) );
    smirror->setValue       ( cfg->lookupValue ( "ui.eye.mirror", 9 ) );
    swidth->setRange        ( 0, 10000 );
    swidth->setValue        ( cfg->lookupValue ( "ui.window.size.width", 320 ) );
    sheight->setRange       ( 0, 10000 );
    sheight->setValue       ( cfg->lookupValue ( "ui.window.size.height", 80) );

    QGridLayout * geyesl = new QGridLayout ();
    geyesl->setSpacing ( 10 );
    geyesl->addWidget  ( new QLabel ( "color" ), 0, 0, 1, 1 );
    geyesl->addWidget  ( new QLabel ( "theme" ), 1, 0, 2, 1 );
    geyesl->addWidget  ( new QLabel ( "eye size" ), 0, 2, 1, 3 );
    geyesl->addWidget  ( new QLabel ( "mirror size" ),1, 2, 2, 3 );
    geyesl->addWidget  ( new QLabel ( "width" ), 2, 2, 3, 3 );
    geyesl->addWidget  ( new QLabel ( "height" ), 3, 2, 4, 3 );
    geyesl->addWidget  ( ccolor, 0, 1, 1, 2 );
    geyesl->addWidget  ( ctheme, 1, 1, 2, 2 );
    geyesl->addWidget  ( seye, 0, 3, 1, 4 );
    geyesl->addWidget  ( smirror, 1, 3, 2, 4 );
    geyesl->addWidget  ( swidth, 2, 3, 3, 4 );
    geyesl->addWidget  ( sheight, 3, 3, 4, 4 );
    geyes->setLayout   ( geyesl );

    sldxmax = new QSpinBox ();
    sldxmin = new QSpinBox ();
    sldymax = new QSpinBox ();
    sldymin = new QSpinBox ();
    sldmax  = new QSpinBox ();
    sldmin  = new QSpinBox ();

    QGridLayout * glookerl = new QGridLayout ();
    glookerl->setSpacing ( 10 );
    glookerl->addWidget  ( new QLabel ( "delta x" ), 0, 0, 1, 2, Qt::AlignHCenter );
    glookerl->addWidget  ( new QLabel ( "min" ), 1, 0, 2, 1 );
    glookerl->addWidget  ( new QLabel ( "max" ), 2, 0, 3, 1 );
    glookerl->addWidget  ( new QLabel ( "delta y" ), 0, 2, 1, 3, Qt::AlignHCenter );
    glookerl->addWidget  ( new QLabel ( "min" ), 1, 2, 2, 3 );
    glookerl->addWidget  ( new QLabel ( "max" ), 2, 2, 3, 3 );
    glookerl->addWidget  ( new QLabel ( "delay" ), 0, 4, 1, 6, Qt::AlignHCenter );
    glookerl->addWidget  ( new QLabel ( "min" ), 1, 4, 2, 5 );
    glookerl->addWidget  ( new QLabel ( "max" ), 2, 4, 3, 5 );
    glookerl->addWidget  ( sldxmin, 1, 1, 2, 2 );
    glookerl->addWidget  ( sldxmax, 2, 1, 3, 2 );
    glookerl->addWidget  ( sldymin, 1, 3, 2, 4 );
    glookerl->addWidget  ( sldymax, 2, 3, 3, 4 );
    glookerl->addWidget  ( sldmin, 1, 5, 2, 6 );
    glookerl->addWidget  ( sldmax, 2, 5, 3, 6 );
    glooker->setLayout   ( glookerl );

    scdmin = new QSpinBox ();
    scdmax = new QSpinBox ();

    QGridLayout * gclapperl = new QGridLayout ();
    gclapperl->setSpacing ( 10 );
    gclapperl->addWidget  ( new QLabel ( "delay" ), 0, 0, 1, 2, Qt::AlignHCenter );
    gclapperl->addWidget  ( new QLabel ( "min" ), 1, 0, 1, 1 );
    gclapperl->addWidget  ( new QLabel ( "max" ), 2, 0, 3, 1 );
    gclapperl->addWidget  ( scdmin, 1, 1, 2, 2 );
    gclapperl->addWidget  ( scdmax, 2, 1, 3, 2 );
    gclapper->setLayout   ( gclapperl );
    info << "(Configurator) done!\n";
}
