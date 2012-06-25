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

#include <iostream>
#include <QApplication>
#include "window.hxx"
#include "eyes.hxx"
#include <debug.hxx>

using namespace std;

eyes_window::eyes_window ( QString color, QWidget * parent ) : QWidget ( parent, Qt::WindowStaysOnTopHint | Qt::FramelessWindowHint )
{
    info << "(window) preparing...\n";
    Configuration * cfg = Configuration::getInstance ();
    float size_multipler = cfg->lookupValue ( "ui.window.height", 80 )/80.0;
    eyes_w = 320*size_multipler;
    eyes_h = 80*size_multipler;
    isicon = cfg->lookupValue ( "ui.window.tray_icon", true );
    resize ( eyes_w, eyes_h );
    setWindowTitle ( "!eyesy!" );
    setAttribute ( Qt::WA_TranslucentBackground, true );
    setAttribute ( Qt::WA_MouseTracking, true );
    //setWindowFlags ( Qt::SplashScreen );
    eyes = new eyes_view ( this, color );
    config = new Configurator ( (QWidget*)nil );
    info << "(window) loading icon " << (eyes->theme + QString ("icon") + eyes->get_color_suffix () + ".png" ).toStdString () << ".\n";
    QPixmap tmp;
    tmp.load ( eyes->theme + QString ("icon") + eyes->get_color_suffix () + ".png" );
    tico = new QIcon ( tmp );
    setWindowIcon ( *tico );
    if ( tmp.isNull () )
    {
        error << "(window) file not found. Continuing whiwaut tray icon.\n";
        isicon = false;
    }
    if ( isicon )
    {
        info << "(window) preparing tray icon...\n";
        trayico = new QSystemTrayIcon ( this );
        trayico->setIcon ( *tico );
        trayico->setToolTip ( "!eyesy!" );
        config->set_icon ( tico );
        //actions
        setA = new QAction ( tr ( "&Settings" ), this );
        connect ( setA, SIGNAL ( triggered () ), config, SLOT ( show () ) );
        quitA = new QAction ( tr ( "&Quit" ), this );
        connect ( quitA, SIGNAL ( triggered () ), this, SLOT ( onQuit () ) );
        //menu
        timenu = new QMenu ( this );
        timenu->addAction ( setA );
        timenu->addSeparator ();
        timenu->addAction ( quitA );
        //iconend
        trayico->setContextMenu ( timenu );
        trayico->show ();
        info << "(window) tray icon ready!\n";
    }
    info << "(eyes_window) menu setup.\n";
    menu = new eyes_menu ( this );
    connect ( menu, SIGNAL ( quitting () ), this, SLOT ( onQuit () ) );
    info << "(eyes_widnow) ready!\n";
    eyes->show ();
    eyes->update ();
}

void eyes_window::mousePressEvent ( QMouseEvent * ev )
{
  if ( ev->button () & Qt::RightButton )
  {
    menu->show ();
    //timenu->show ();
    //timenu->move ( x () - timenu->width () + width (), y () );
  }
}

void eyes_window::onQuit ()
{
  Configuration::getInstance ()->save ();
  qApp->quit ();
}
