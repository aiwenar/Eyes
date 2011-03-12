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
    if ( isicon )
    {
        cerr << "[info :] loading tray icon " << ( QString ( "./pics/trayico" ) + eyes->get_color_suffix () + ".png" ).toStdString () << ".\n";
        QPixmap tmp;
        tmp.load ( QString ( "./pics/trayico" ) + eyes->get_color_suffix () + ".png" );
        if ( tmp.isNull () )
        {
            cerr << "[\033[31merror \033[0m:] file not found. Continuing whiwaut tray icon.\n";
            isicon = false;
        }
        else
        {
            tico = new QIcon ( tmp );
            trayico = new QSystemTrayIcon ( this );
            trayico->setIcon ( *tico );
            trayico->setToolTip ( "!eyesy!" );
            //actions
            quitA = new QAction ( tr ( "&Quit" ), this );
            connect ( quitA, SIGNAL ( triggered () ), qApp, SLOT ( quit () ) );
            //menu
            timenu = new QMenu ( this );
            timenu->addSeparator ();
            timenu->addAction ( quitA );
            //iconend
            trayico->setContextMenu ( timenu );
            trayico->show ();
        }
    }
    eyes->show ();
    eyes->update ();
}
