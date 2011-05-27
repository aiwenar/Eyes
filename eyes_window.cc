#include <iostream>
#include <QApplication>
#include "eyes_window.hxx"
#include "eyes_info.h"
#include <debug.hxx>

using namespace std;

eyes_window::eyes_window ( QString color, QWidget * parent ) : QWidget ( parent, Qt::WindowStaysOnTopHint | Qt::FramelessWindowHint )
{
    cfg = new eConfig ( "config.cfg" );
    if ( cfg->lookupValue ( "debug.info.on", false ) )
    {
        info.open ( cfg->lookupValue ( "debug.info.out", "/dev/stderr" ) );
        info.set_str ( "info" );
    }
    else
    {
        info.open ( "/dev/null" );
        info.set_str ( "info" );
    }
    if ( cfg->lookupValue ( "debug.warning.on", false ) )
    {
        warning.open ( cfg->lookupValue ( "debug.warning.out", "/dev/stderr" ) );
        warning.set_str ( "/033[33mwarning\033[0m" );
    }
    else
    {
        warning.open ( "/dev/null" );
        warning.set_str ( "warning" );
    }
    if ( cfg->lookupValue ( "debug.error.on", false ) )
    {
        error.open ( cfg->lookupValue ( "debug.error.out", "/dev/stderr" ) );
        error.set_str ( "/033[31merror/033[0m" );
    }
    else
    {
        error.open ( "/dev/null" );
        error.set_str ( "error" );
    }
    eyes_w = cfg->lookupValue ( "ui.window.size.width", 320 );
    eyes_h = cfg->lookupValue ( "ui.window.size.height", 80 );
    isicon = cfg->lookupValue ( "ui.window.tray_icon", true );
    resize ( eyes_w, eyes_h );
    setWindowTitle ( "!eyesy!" );
    setAttribute ( Qt::WA_TranslucentBackground, true );
    eyes = new eyes_view ( this, color );
    ecfg = new eyes_config ( *cfg, (QWidget*)nil );
    ecfg->set_icon ( eyes->get_color_suffix () );
    info << "loading icon " << ( QString ( "./pics/icon" ) + eyes->get_color_suffix () + ".png" ).toStdString () << ".\n";
    QPixmap tmp;
    tmp.load ( QString ( "./pics/icon" ) + eyes->get_color_suffix () + ".png" );
    tico = new QIcon ( tmp );
    setWindowIcon ( *tico );
    if ( tmp.isNull () )
    {
        error << "file not found. Continuing whiwaut tray icon.\n";
        isicon = false;
    }
    if ( isicon )
    {
        trayico = new QSystemTrayIcon ( this );
        trayico->setIcon ( *tico );
        trayico->setToolTip ( "!eyesy!" );
        //actions
        setA = new QAction ( tr ( "&Settings" ), this );
        connect ( setA, SIGNAL ( triggered () ), ecfg, SLOT ( show () ) );
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
    }
    eyes->show ();
    eyes->update ();
}
