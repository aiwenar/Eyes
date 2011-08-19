#include <iostream>
#include <QApplication>
#include "eyes_window.hxx"
#include "eyes_info.h"
#include <debug.hxx>

using namespace std;

eyes_window::eyes_window ( QString color, QWidget * parent ) : QWidget ( parent, Qt::WindowStaysOnTopHint | Qt::FramelessWindowHint )
{
    info << "(eyes_window) preparing...\n";
    oldcfg = new eConfig ( "config.cfg" );
    Configuration * cfg = Configuration::getInstance ();
    eyes_w = cfg->lookupValue ( "ui.window.size.width", 320 );
    eyes_h = cfg->lookupValue ( "ui.window.size.height", 80 );
    isicon = cfg->lookupValue ( "ui.window.tray_icon", true );
    resize ( eyes_w, eyes_h );
    setWindowTitle ( "!eyesy!" );
    setAttribute ( Qt::WA_TranslucentBackground, true );
    //setWindowFlags ( Qt::SplashScreen );
    eyes = new eyes_view ( this, color );
    ecfg = new eyes_config ( *oldcfg, (QWidget*)nil );
    config = new Configurator ( (QWidget*)nil );
    ecfg->set_icon ( eyes->get_color_suffix () );
    info << "(eyes_window) loading icon " << ( QString ( "./pics/icon" ) + eyes->get_color_suffix () + ".png" ).toStdString () << ".\n";
    QPixmap tmp;
    tmp.load ( QString ( "./pics/icon" ) + eyes->get_color_suffix () + ".png" );
    tico = new QIcon ( tmp );
    setWindowIcon ( *tico );
    if ( tmp.isNull () )
    {
        error << "(eyes_window) file not found. Continuing whiwaut tray icon.\n";
        isicon = false;
    }
    if ( isicon )
    {
        info << "(eyes_window) preparing tray icon...\n";
        trayico = new QSystemTrayIcon ( this );
        trayico->setIcon ( *tico );
        trayico->setToolTip ( "!eyesy!" );
        config->set_icon ( tico );
        //actions
        setA = new QAction ( tr ( "&Settings" ), this );
        connect ( setA, SIGNAL ( triggered () ), config, SLOT ( show () ) );
        quitA = new QAction ( tr ( "&Quit" ), this );
        connect ( quitA, SIGNAL ( triggered () ), qApp, SLOT ( quit () ) );
        //menu
        timenu = new QMenu ( this );
        timenu->addAction ( setA );
        timenu->addSeparator ();
        timenu->addAction ( quitA );
        //iconend
        trayico->setContextMenu ( timenu );
        trayico->show ();
        info << "(eyes_window) tray icon ready!\n";
    }
    info << "(eyes_widnow) ready!\n";
    eyes->show ();
    eyes->update ();
}
