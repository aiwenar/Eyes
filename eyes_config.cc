#include <QPainter>
#include <QScrollArea>
#include <iostream>
#include "eyes_config.hxx"

using namespace std;
using namespace eyes_cfg_v;

const static key_info keys [] = {
    //path to key                   key type   key description
    { "ui.color",                   "string",  "Specyfics eyes color." },
    { "ui.debug",                   "boolean", "Specyfics that gui log shall be written on terminal (or file) or no." },
    { "ui.eye.size",                "integer", "Specyfics eye size." },
    { "ui.eye.mirror",              "integer", "Specyfics eye mirror size." },
    { "ui.window.tray_icon",        "boolean", "Specyfics is toolbar icon show." },
    { "ui.window.size.width",       "integer", "Specyfics window width {WARNING: currenty not working}" },
    { "ui.window.size.height",      "integer", "Specyfics window height {WARNING: currently not working}" },
    { "ui.looker.min_dx",           "integer", "Specyfics minimal eyes x position." },
    { "ui.looker.max_dx",           "integer", "Specyfics maximal eyes x position." },
    { "ui.looker.min_dy",           "integer", "Specyfics minimal eyes y position." },
    { "ui.looker.max_dx",           "integer", "Specyfics maximal etes y position." },
    { "ui.looker.min_delay",        "integer", "Specyfics minimal delay between looks. (this is (ui.looker.min_delay)*200 milisecounds)" },
    { "ui.looker.max_delay",        "integer", "Specyfics maximal delay between looks. (this is (ui.looker.max_delay_*200 milisecounds)" },
    { "ui.clapper.min_delay",       "integer", "Specyfics minimal delay between claps. (this is (ui.looker.min_delay)*200 milisecounds)" },
    { "ui.clapper.max_delay",       "integer", "Specyfics maximal delay between claps. (this is (ui.looker.max_delay_*200 milisecounds)" }
};

const static int keys_count = 15;

eyes_config::eyes_config ( eConfig& ncfg, QWidget * parent ) : QWidget ( parent )
{
    resize ( 450, 230 );
    setWindowTitle ( QString ( "!eyes! config" ) );
    cfg = &ncfg;
    tabs = new QTabWidget ( this );
    tabs->setTabPosition ( QTabWidget::East );
    adv = new cfg_advanced ( ncfg, tabs );
    tabs->addTab ( adv, QString ( "advanced" ) );
    adv->show ();
}

void eyes_config::set_icon ( QString suffix )
{
    QPixmap icn;
    icn.load ( QString  ( "./pics/icon" ) + suffix + ".png" );
    if ( icn.isNull () )
    {
        cerr << "[\033[31merror \033[0m:] file " << ( QString ( "./pics/icon" ) + suffix + ".png" ).toStdString () << " not found.\n";
        return;
    }
    QPixmap tool;
    tool.load ( QString ( "./pics/config.png" ) );
    if ( icn.isNull () )
    {
        cerr << "[\033[31merror \033[0m:] file " << ( QString ( "./pics/config.png" ) ).toStdString () << " not found.\n";
        return;
    }
    QPainter paint ( &icn );
    paint.drawPixmap ( 130, 130, 130, 130, tool );
    paint.end ();
    icon = new QIcon ( icn );
    setWindowIcon ( *icon );
}

cfga_item::cfga_item ( const key_info& what, Config * cfg, QWidget * parent ) : QWidget ( parent )
{
    hbl = new QHBoxLayout ( this );
    path = new QLabel ( what.path, this );
    info = &what;
    try
    {
        set = &cfg->lookup ( what.path );
    }
    catch ( SettingNotFoundException e )
    {
        cerr << "Error occured when runing eyes. Here you have raport:\n" << e.what () << " " << e.getPath () << "\n\n";
        exit ( -1 );
    }
    tedit = new QLineEdit ( this );
    tedit->setBaseSize ( 300, 30 );
    hbl->addWidget ( path );
    hbl->addWidget ( tedit );
    setToolTip ( QString ( what.desc ) );
    resize ( 600, 30 );
}

void cfga_item::show ()
{
    path->show ();
    tedit->show ();
}

cfg_advanced::cfg_advanced ( eConfig& ncfg, QWidget * parent ) : QWidget ( parent )
{
    cfg = &ncfg;
    list = new QVBoxLayout ( this );
    list->setSpacing ( 0 );
    Config * lcfg = cfg->libconfigConfig ();
    for ( int i=0 ; i<keys_count ; i++ )
    {
        items.push_back ( new cfga_item ( keys[i], lcfg, this ) );
        list->addWidget ( items[i] );
    }
}

void cfg_advanced::show ()
{
    for ( int i=0 ; i<items.count() ; i++ )
    {
        items[i]->show ();
    }
}
