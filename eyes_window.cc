//
#include <iostream>
#include <QApplication>
#include "eyes_window.hxx"
#include "eyes_info.h"

using namespace std;

eyes_window::eyes_window ( QString color, QWidget * parent ) : QWidget ( parent, Qt::WindowStaysOnTopHint | Qt::FramelessWindowHint )
{
    cfg = new Config ();
    cfg->readFile ( "config.cfg" );
    if ( not cfg->lookupValue ( "ui.window.size.width", eyes_w ) or
         not cfg->lookupValue ( "ui.window.size.height", eyes_h ) )
    {
        cerr << "[\033[31merror \033[0m:] section ui.window.size in configuration file is invalid.\n";
        exit ( 126 );
    }
    if ( not cfg->lookupValue ( "ui.window.tray_icon", isicon ) )
    {
        cerr << "\033[31merror \033[0m:] variable ui.window.tray_icon not found.\n";
        exit ( 126 );
    }
    resize ( eyes_w, eyes_h );
    setWindowTitle ( "!eyesy!" );
    setAttribute ( Qt::WA_TranslucentBackground, true );
    eyes = new eyes_view ( this, color );
    ecfg = new eyes_config ( *cfg, (QWidget*)nil );
    ecfg->set_icon ( eyes->get_color_suffix () );
    cerr << "[info :] loading icon " << ( QString ( "./pics/icon" ) + eyes->get_color_suffix () + ".png" ).toStdString () << ".\n";
    QPixmap tmp;
    tmp.load ( QString ( "./pics/icon" ) + eyes->get_color_suffix () + ".png" );
    tico = new QIcon ( tmp );
    setWindowIcon ( *tico );
    if ( tmp.isNull () )
    {
        cerr << "[\033[31merror \033[0m:] file not found. Continuing whiwaut tray icon.\n";
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
