#include "eyes_view.h"
#include <debug.hxx>

#include <iostream>

using namespace std;

eyes_looker::eyes_looker ( eyes_view * neyes )
{
    eyes = neyes;
    timer = new QTimer ( this );
    connect ( timer, SIGNAL ( timeout() ), this, SLOT ( look() ) );
    timer->setInterval ( ( qrand () % 30 + 1 )*200 );
    info << "eyes_looker working.\n";
}

void eyes_looker::load_config ( eConfig * set )
{
    min_dx = set->lookupValue ( "ui.looker.delta_x.min", 0 );
    max_dx = set->lookupValue ( "ui.looker.delta_x.max", 14 );
    min_dy = set->lookupValue ( "ui.looker.delta_y.min", 0 );
    max_dy = set->lookupValue ( "ui.looker.delta_y.max", 10 );
    bmin_x = set->lookupValue ( "ui.looker.bounds_x.min", 40 );
    bmax_x = set->lookupValue ( "ui.looker.bounds_x.max", 54 );
    bmin_y = set->lookupValue ( "ui.looker.bounds_y.min", 4 );
    bmax_y = set->lookupValue ( "ui.looker.bounds_y.max", 14 );
    min_dl = set->lookupValue ( "ui.looker.delay.min", 1 );
    max_dl = set->lookupValue ( "ui.looker.delay.max", 30 );
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
    eyes->set_mirror_position ( mpx1+(dx!=0?(double(dx)/3):0), mpx2+(dx!=0?(double(dx)/3):0), mpy+(dy!=0?(double(dy)/3):0) );
    info << "looking witch dx:" << dx << " and dy:" << dy << " mirrors at mdx:" << (dx!=0?(double(dx)/3):0) << " and mdy:" << (dy!=0?(double(dy)/3):0) << ".\n";
    timer->setInterval ( ( qrand() % max_dl + min_dl ) * 200 );
    eyes->repaint ();
}
