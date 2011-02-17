#include <QTime>
#include <QChar>
#include <iostream>
#include "eyes_view.h"

using namespace std;

static char *claps[] = {
    "clap_01", "bul_03", "clap_02", "clap_03", "clap_04", "clap_05", "clap_04", "clap_03", "clap_02", "clap_01"
};

eyes_clapper::eyes_clapper ( eyes_view * neyes )
{
    cerr << "[info :] setting up eyes_clapper.\n";
    eyes = neyes;
    timer = new QTimer ( this );
    connect ( timer, SIGNAL ( timeout () ), this, SLOT ( clap () ) );
    time = QTime::currentTime ();
    qsrand ( (uint)time.msec () );
    stage = 0;
    cerr << "[info :] eyes_clapper is ready.\n";
}

int eyes_clapper::get_next_clap_delay ()
{
    return ( qrand () % 31 + 5 );
}

void eyes_clapper::run ()
{
    timer->start ( get_next_clap_delay () * 200 );
    cerr << "[info :] starting eyes_clapper.\n";
}

void eyes_clapper::clap ()
{
    if ( stage == 0 )
        eyes->lock_face ( this );
    eyes->set_face ( claps[stage], this );
    eyes->repaint ();
    if ( stage == 10 )
    {
        eyes->unlock_face ( this );
        timer->setInterval ( get_next_clap_delay () * 200 );
        stage = 0;
        return;
    }
    stage ++;
    timer->setInterval ( 15 );
}
