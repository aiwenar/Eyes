#ifndef _eyes_clap_animations_hxx
#define _eyes_clap_animations_hxx

#include <QMap>

// dodawaj animki z nazwami anim_<name>_[<nr>_]_<open/close>
// a potem updatuj w register_animations

static char * anim_bul_01_close[] = {

};

static char * anim_bul_01_open[] = {

};


void register_animations ( QMap <QString,char**> * map )
{
    map->insert ( "bul_01_open", anim_bul_01_open );
    map->insert ( "bul_01_close", anim_bul_01_close );
}

#endif //eyes_clap_animations_hxx
