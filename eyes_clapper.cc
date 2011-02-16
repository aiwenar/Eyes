#include <QTime>
#include "eyes_view.h"

eyes_clapper::eyes_clapper ( eyes_view * neyes )
{
    eyes = neyes;
    //timer = new QTimer ( this );
    //connect ( timer, SIGNAL ( timeout () ), this, SLOT ( clap () ) );
    //time = QTime::currentTime ();
    //qsrand ( (uint)time.msec () );
}

int eyes_clapper::get_next_clap_delay ()
{
    return ( qrand () % 31 + 5 );
}

void eyes_clapper::run ()
{
    timer->start ( get_next_clap_delay () );
}

void eyes_clapper::clap ()
{
    for ( int i=0 ; i<6 ; i++ )
    {
        eyes->set_face ( QString ( "clap_0" ) + i );
        eyes->update ();
        msleep ( 200 );
    }
    for ( int i=5 ; i>=0 ; i++ )
    {
        eyes->set_face ( QString ( "clap_0" ) + i );
        eyes->update ();
        msleep ( 200 );
    }
    timer->setInterval ( get_next_clap_delay () );
}
