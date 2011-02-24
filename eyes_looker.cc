#include "eyes_view.h"
#include <iostream>

using namespace std;

eyes_looker::eyes_looker ( eyes_view * neyes )
{
    eyes = neyes;
    timer = new QTimer ( this );
    connect ( timer, SIGNAL ( timeout() ), this, SLOT ( look() ) );
    timer->setInterval ( ( qrand () % 30 + 1 )*200 );
    timer->start ();
    cerr << "[info :] eyes_looker working.\n";
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
    if ( px1+dx < 20 or px1+dx > 80 )
        dx = 0;
    if ( py+dy < 0 or py+dy > 30 )
        dy = 0;
    eyes->set_eyes_position ( px1+dx, px2+dx, py+dy );
    cerr << "[info :] looking witch dx:" << dx << " and dy:" << dy << ".\n";
    timer->setInterval ( ( qrand() % 30 + 1 ) * 200 );
    eyes->repaint ();
}
