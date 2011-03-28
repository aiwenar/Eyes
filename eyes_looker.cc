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
    if ( not set->lookupValue ( "ui.looker.delta_x.min", min_dx ) or
         not set->lookupValue ( "ui.looker.delta_x.max", max_dx ) or
         not set->lookupValue ( "ui.looker.delta_y.min", min_dy ) or
         not set->lookupValue ( "ui.looker.delta_y.max", max_dy ) or
         not set->lookupValue ( "ui.looker.bounds_x.min", bmin_x ) or
         not set->lookupValue ( "ui.looker.bounds_x.max", bmax_x ) or
         not set->lookupValue ( "ui.looker.bounds_y.min", bmin_y ) or
         not set->lookupValue ( "ui.looker.bounds_y.max", bmax_y ) or
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
    static double mpx1;
    static double mpx2;
    static double mpy;
    px1 = eyes->get_eyes_x1 ();
    px2 = eyes->get_eyes_x2 ();
    py = eyes->get_eyes_y ();
    mpx1 = eyes->get_mirror_x1 ();
    mpx2 = eyes->get_mirror_x2 ();
    mpy = eyes->get_mirror_y ();
    dx = ( (qrand()%100+1) > 50 ? -1 : 1 )*(qrand() % max_dx + min_dx);
    dy = ( (qrand()%100+1) > 50 ? -1 : 1 )*(qrand() % max_dy + min_dy);
    if ( px1+dx < bmin_x or px1+dx > bmax_x )
        dx = 0;
    if ( py+dy < bmin_y or py+dy > bmax_y )
        dy = 0;
    eyes->set_eyes_position ( px1+dx, px2+dx, py+dy );
    eyes->set_mirror_position ( mpx1+(dx!=0?(double(dx)/10):0), mpx2+(dx!=0?(double(dx)/10):0), mpy+(dy!=0?(double(dy)/10):0) );
    cerr << "[info :] looking witch dx:" << dx << " and dy:" << dy << " mirrors at mdx:" << (dx!=0?(double(dx)/10):0) << " and mdy:" << (dy!=0?(double(dy)/10):0) << ".\n";
    timer->setInterval ( ( qrand() % max_dl + min_dl ) * 200 );
    eyes->repaint ();
}
