#include "eyes_view.h"
#include <iostream>

using namespace std;

eyes_looker::eyes_looker ( eyes_view * neyes )
{
    eyes = neyes;
    timer = new QTimer ( this );
    connect ( timer, SIGNAL ( timeout() ), this, SLOT ( look() ) );
    timer->setInterval ( ( qrand () % 30 + 1 )*200 );
    cerr << "[info :] eyes_looker working.\n";
}

void eyes_looker::load_config ( Config * set )
{
    if ( not set->lookupValue ( "ui.looker.min_dx", min_dx ) or
         not set->lookupValue ( "ui.looker.max_dx", max_dx ) or
         not set->lookupValue ( "ui.looker.min_dy", min_dy ) or
         not set->lookupValue ( "ui.looker.max_dy", max_dy ) or
         not set->lookupValue ( "ui.looker.min_delay", min_dl ) or
         not set->lookupValue ( "ui.looker.max_delay", max_dl ) )
    {
        cerr << "[\033[31merror \033[0m:] ui.looker section in config is invalid.\n";
        exit ( 126 );
    }
}

void eyes_looker::run ()
{
    timer->start ();
}

void eyes_looker::look ()
{
    static int dx;
    static int dy;
    static int px1;
    static int px2;
    static int py;
    dx = ( (qrand()%100+1) > 50 ? -1 : 1 )*(qrand() % 40 + 1);
    dy = ( (qrand()%100+1) > 50 ? -1 : 1 )*(qrand() % 40 + 1);
    px1 = eyes->get_eyes_x1 ();
    px2 = eyes->get_eyes_x2 ();
    py = eyes->get_eyes_y ();
    if ( px1+dx < min_dx or px1+dx > max_dx )
        dx = 0;
    if ( py+dy < min_dy or py+dy > max_dy )
        dy = 0;
    eyes->set_eyes_position ( px1+dx, px2+dx, py+dy );
    cerr << "[info :] looking witch dx:" << dx << " and dy:" << dy << ".\n";
    timer->setInterval ( ( qrand() % max_dl + min_dl ) * 200 );
    eyes->repaint ();
}
