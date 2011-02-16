#include <QTime>
#include <QChar>
#include <iostream>
#include "eyes_view.h"

using namespace std;

static char *claps[] = {
    "clap_01", "clap_02", "clap_03", "clap_04", "clap_05"
};

eyes_clapper::eyes_clapper ( eyes_view * neyes )
{
    cerr << "[info :] setting up eyes_clapper.\n";
    eyes = neyes;
    timer = new QTimer ( this );
    connect ( timer, SIGNAL ( timeout () ), this, SLOT ( clap () ) );
    time = QTime::currentTime ();
    qsrand ( (uint)time.msec () );
    cerr << "[info :] eyes_clapper is rady.\n";
}

int eyes_clapper::get_next_clap_delay ()
{
    return ( qrand () % 31 + 5 );
}

void eyes_clapper::run ()
{
    timer->start ( get_next_clap_delay () );
    cerr << "[info :] starting eyes_clapper.\n";
}

void eyes_clapper::clap ()
{
    static int delay;
    eyes->lock_face ( this );
    for ( int i=0 ; i<5 ; i++ )
    {
        eyes->set_face ( QString ( claps[i] ), this );
        eyes->update ();
        msleep ( 200 );
    }
    for ( int i=4 ; i>=0 ; i-- ) // from '5' to '0'
    {
        eyes->set_face ( QString ( claps[i] ) += i, this );
        eyes->update ();
        msleep ( 200 );
    }
    eyes->unlock_face ( this );
    delay = get_next_clap_delay () * 200;
    cerr << "[info :] seting next clap delay to " << delay << ".\n";
    timer->setInterval ( delay );
}
