#include <QTime>
#include <QChar>
#include <iostream>

#include "eyes_view.h"
#include "clap_animations.hxx"
#include <debug.hxx>

using namespace std;

// actually unused.
static char *claps[] = {
    "clap_01", "clap_02", "clap_03", "clap_04", "clap_05", "clap_04", "clap_03", "clap_02", "clap_01",
};

eyes_clapper::eyes_clapper ( eyes_view * neyes )
{
    info << "(eyes_clapper) preparing...\n";
    register_animations ( &animations );
    eyes = neyes;
    timer = new QTimer ( this );
    connect ( timer, SIGNAL ( timeout () ), this, SLOT ( clap () ) );
    stage = 0;
    info << "(eyes_clapper) loading config...\n";
    Configuration * cfg = Configuration::getInstance ();
    min_dl = cfg->lookupValue ( "ui.clapper.delay.min", 5 );
    max_dl = cfg->lookupValue ( "ui.clapper.delay.max", 35 );
    info << "(eyes_clapper) ready!\n";
    start = "slp_10_open";
    end = "slp_10_close";
    from = 0;
    to = 0;
}

int eyes_clapper::get_next_clap_delay ()
{
    return ( qrand () % max_dl + min_dl );
}

void eyes_clapper::run ()
{
    timer->start ( get_next_clap_delay () * 200 );
    info << "starting eyes_clapper.\n";
}

void eyes_clapper::set_animation ( QString nstart, QString nend, int nfrom, int nto )
{
    if ( not animations.contains ( nstart ) or not animations.contains ( nend ) )
    {
        error << "(eyes_clapper) seting animation from " << nstart.toStdString () << " to " << nend.toStdString () << " but one nit exists.\n";
        return;
    }
    if ( nto >= animations[nend]->size )
        to = animations[nend]->size;
    else
        to = nto;
    start = nstart;
    end = nend;
    from = nfrom;
}

void eyes_clapper::clap ()
{
    if ( stage == from )
    {
        face = eyes->get_face_next ();
        size1 = animations[start]->size;
        size2 = animations[end]->size;
        eyes->reload_eyes();
    }
    if ( stage < size1 )
        eyes->set_face ( QString ( animations[start]->frames[stage] ) );

    else
    {
        if ( stage-size1 == to )
        {
            eyes->set_face ( face );
            eyes->update ();
            timer->setInterval ( get_next_clap_delay () * 200 );
            stage = from;
            return;
        }
        eyes->set_face ( QString ( animations[end]->frames[stage-size1] ) );
    }
    eyes->repaint ();
    stage ++;
    eyes->anims_reload();
    timer->setInterval ( 20 );
}
